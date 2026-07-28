import os
import joblib
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, roc_auc_score

# Define directories
DATASET_PATH = os.path.join("ml", "datasets", "PhiUSIIL_Phishing_URL_Dataset.csv")
ML_MODELS_DIR = os.path.join("ml", "models")
BACKEND_MODELS_DIR = os.path.join("backend", "models")

# Ensure directories exist
os.makedirs(ML_MODELS_DIR, exist_ok=True)
os.makedirs(BACKEND_MODELS_DIR, exist_ok=True)

# List of 41 features that can be extracted from a live web page and URL
DEPLOYMENT_FEATURES = [
    'URLLength', 'DomainLength', 'IsDomainIP', 'NoOfSubDomain', 'NoOfLettersInURL', 
    'LetterRatioInURL', 'NoOfDegitsInURL', 'DegitRatioInURL', 'NoOfEqualsInURL', 
    'NoOfQMarkInURL', 'NoOfAmpersandInURL', 'NoOfOtherSpecialCharsInURL', 'SpacialCharRatioInURL', 
    'IsHTTPS', 'LineOfCode', 'LargestLineLength', 'HasTitle', 'DomainTitleMatchScore', 
    'URLTitleMatchScore', 'HasFavicon', 'IsResponsive', 'NoOfURLRedirect', 'NoOfSelfRedirect', 
    'HasDescription', 'NoOfPopup', 'NoOfiFrame', 'HasExternalFormSubmit', 'HasSocialNet', 
    'HasSubmitButton', 'HasHiddenFields', 'HasPasswordField', 'Bank', 'Pay', 'Crypto', 
    'HasCopyrightInfo', 'NoOfImage', 'NoOfCSS', 'NoOfJS', 'NoOfSelfRef', 'NoOfEmptyRef', 
    'NoOfExternalRef'
]

TARGET = 'label'

def main():
    print(f"Loading dataset from {DATASET_PATH}...")
    if not os.path.exists(DATASET_PATH):
        raise FileNotFoundError(f"Dataset not found at {DATASET_PATH}. Please make sure it is moved correctly.")
        
    df = pd.read_csv(DATASET_PATH)
    print(f"Dataset shape: {df.shape}")
    
    # Filter features
    X = df[DEPLOYMENT_FEATURES]
    y = df[TARGET]
    
    print(f"Training deployment model using {len(DEPLOYMENT_FEATURES)} features...")
    
    # Split FIRST to prevent data leakage
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, 
        test_size=0.2, 
        random_state=42, 
        stratify=y
    )
    
    # Scale AFTER split
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    
    # Train Random Forest Classifier
    # We restrict max_depth to 15 to keep model size reasonable while maintaining high accuracy
    model = RandomForestClassifier(
        n_estimators=100, 
        max_depth=15, 
        random_state=42, 
        n_jobs=-1
    )
    
    print("Fitting model...")
    model.fit(X_train_scaled, y_train)
    
    # Evaluate
    print("Evaluating model...")
    y_pred = model.predict(X_test_scaled)
    y_prob = model.predict_proba(X_test_scaled)[:, 1]
    
    accuracy = accuracy_score(y_test, y_pred)
    precision = precision_score(y_test, y_pred)
    recall = recall_score(y_test, y_pred)
    f1 = f1_score(y_test, y_pred)
    roc_auc = roc_auc_score(y_test, y_prob)
    
    print("="*60)
    print("DEPLOYMENT MODEL PERFORMANCE")
    print("="*60)
    print(f"Accuracy:  {accuracy:.6f}")
    print(f"Precision: {precision:.6f}")
    print(f"Recall:    {recall:.6f}")
    print(f"F1 Score:  {f1:.6f}")
    print(f"ROC-AUC:   {roc_auc:.6f}")
    print("="*60)
    
    # Save artifacts
    for directory in [ML_MODELS_DIR, BACKEND_MODELS_DIR]:
        model_path = os.path.join(directory, "deployment_model.pkl")
        preprocessor_path = os.path.join(directory, "preprocessor.pkl")
        features_path = os.path.join(directory, "feature_columns.pkl")
        
        joblib.dump(model, model_path)
        joblib.dump(scaler, preprocessor_path)
        joblib.dump(DEPLOYMENT_FEATURES, features_path)
        print(f"Saved artifacts in: {directory}")
        
    print("Deployment model training complete!")

if __name__ == "__main__":
    main()
