import os
import joblib
import pandas as pd
import numpy as np
import shap

# Paths to models inside backend
MODELS_DIR = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), "models")
MODEL_PATH = os.path.join(MODELS_DIR, "deployment_model.pkl")
PREPROCESSOR_PATH = os.path.join(MODELS_DIR, "preprocessor.pkl")
FEATURES_PATH = os.path.join(MODELS_DIR, "feature_columns.pkl")

# Lazy loading of models to improve startup time
_model = None
_preprocessor = None
_features = None
_explainer = None

def load_prediction_artifacts():
    global _model, _preprocessor, _features, _explainer
    if _model is None:
        if not os.path.exists(MODEL_PATH):
            raise FileNotFoundError(f"Model file not found at {MODEL_PATH}. Run training script first.")
        _model = joblib.load(MODEL_PATH)
        _preprocessor = joblib.load(PREPROCESSOR_PATH)
        _features = joblib.load(FEATURES_PATH)
        # Initialize SHAP explainer for the Random Forest model
        _explainer = shap.TreeExplainer(_model)

def calculate_risk(probability: float) -> tuple[int, str]:
    score = int(probability * 100)
    
    if score <= 20:
        level = "Very Safe"
    elif score <= 40:
        level = "Low Risk"
    elif score <= 60:
        level = "Medium Risk"
    elif score <= 80:
        level = "High Risk"
    else:
        level = "Critical"
        
    return score, level

def get_human_reason(feature_name: str, value: float) -> str:
    # Map feature names to human-readable explanations based on value ranges
    reasons_map = {
        'IsHTTPS': "HTTPS is disabled (unsecure connection).",
        'URLLength': f"URL length is unusually long ({int(value)} characters).",
        'DomainLength': f"Domain name length is high ({int(value)} characters).",
        'IsDomainIP': "Domain is an IP address instead of a standard hostname.",
        'NoOfSubDomain': f"High number of subdomains detected ({int(value)}).",
        'NoOfDegitsInURL': f"Suspicious number of digits in the URL ({int(value)}).",
        'NoOfOtherSpecialCharsInURL': f"Unusual number of special characters in URL ({int(value)}).",
        'SpacialCharRatioInURL': "High ratio of special characters in URL string.",
        'LineOfCode': f"Webpage contains minimal source code ({int(value)} lines).",
        'NoOfPopup': "Page contains scripting that opens popup windows.",
        'NoOfiFrame': "Webpage contains hidden or embedded iframe elements.",
        'HasExternalFormSubmit': "Form submissions are routed to external third-party domains.",
        'HasSocialNet': "No links to standard social networks found.",
        'HasSubmitButton': "Form submit button detected on suspicious page structure.",
        'HasHiddenFields': "Hidden inputs/fields detected in forms.",
        'HasPasswordField': "Password field detected on a suspicious/unsecure page.",
        'Bank': "Page references banking keywords without secure credentials.",
        'Pay': "Page references financial/billing keywords.",
        'Crypto': "Page references cryptocurrency/wallet keywords.",
        'NoOfImage': f"Unusually low number of images ({int(value)}) compared to standard sites.",
        'NoOfCSS': f"Very few styling elements ({int(value)} CSS tags) indicating a raw template.",
        'NoOfJS': f"Very few scripting files ({int(value)} JS tags).",
        'NoOfExternalRef': f"Very few external assets/links ({int(value)} links) implying a self-contained clone.",
        'NoOfEmptyRef': f"High count of empty/placeholder links ({int(value)} empty links)."
    }
    
    # Custom rule evaluations for thresholds where applicable
    if feature_name == 'IsHTTPS' and value == 1:
        return ""
    if feature_name == 'URLLength' and value < 75:
        return ""
    if feature_name == 'DomainLength' and value < 25:
        return ""
    if feature_name == 'IsDomainIP' and value == 0:
        return ""
    if feature_name == 'NoOfSubDomain' and value < 3:
        return ""
    if feature_name == 'NoOfPopup' and value == 0:
        return ""
    if feature_name == 'HasHiddenFields' and value == 0:
        return ""
    if feature_name == 'HasPasswordField' and value == 0:
        return ""
    if feature_name == 'HasExternalFormSubmit' and value == 0:
        return ""
    if feature_name == 'NoOfEmptyRef' and value < 5:
        return ""
    if feature_name == 'NoOfExternalRef' and value > 15:
        return ""
    if feature_name == 'LineOfCode' and value > 150:
        return ""

    return reasons_map.get(feature_name, f"Suspicious indicator in {feature_name}.")

def predict_url_features(features_dict: dict) -> dict:
    load_prediction_artifacts()
    
    # Order features matching model's expected columns
    ordered_row = []
    for col in _features:
        ordered_row.append(features_dict.get(col, 0))
        
    df_row = pd.DataFrame([ordered_row], columns=_features)
    
    # Scale features
    scaled_row = _preprocessor.transform(df_row)
    
    # Predict
    probability = _model.predict_proba(scaled_row)[0][1] # Probability of class 1 (Legitimate)
    prediction_val = _model.predict(scaled_row)[0]
    
    # Class 1 = Legitimate, Class 0 = Phishing
    prediction = "Legitimate" if prediction_val == 1 else "Phishing"
    phishing_prob = 1.0 - probability
    
    score, level = calculate_risk(phishing_prob)
    
    # Calculate SHAP values
    shap_values = _explainer.shap_values(scaled_row)
    
    # Class 0 is Phishing, so we want the contributions towards class 0
    if isinstance(shap_values, list):
        shap_contrib = shap_values[0][0]  # shape (num_features,)
    else:
        # Depending on SHAP version, it might return a 3D array or a single 2D array
        if len(shap_values.shape) == 3:
            shap_contrib = shap_values[0, :, 0]
        else:
            # If it is a log-odds output for class 1, class 0 impact is the negative
            shap_contrib = -shap_values[0]
            
    # Sort features by positive SHAP contribution (meaning they push prediction towards Phishing)
    indices = np.argsort(shap_contrib)[::-1]
    
    reasons = []
    for idx in indices:
        f_name = _features[idx]
        f_val = df_row[f_name].values[0]
        contrib = shap_contrib[idx]
        
        # Only suggest reasons for features that positively contributed to the phishing label
        if contrib > 0.005:
            reason = get_human_reason(f_name, f_val)
            if reason and reason not in reasons:
                reasons.append(reason)
                
        # Limit to top 4 human-readable reasons to keep dashboard tidy
        if len(reasons) >= 4:
            break
            
    if not reasons:
        reasons.append("No major suspicious indicators detected.")
        
    return {
        "prediction": prediction,
        "confidence": round(probability * 100, 2) if prediction == "Legitimate" else round(phishing_prob * 100, 2),
        "risk_score": score,
        "risk_level": level,
        "reasons": reasons
    }
