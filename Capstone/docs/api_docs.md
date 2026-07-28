# API Documentation - PhishShield AI Platform

This document describes the API endpoints provided by the PhishShield FastAPI backend service.

The backend runs locally by default at: `http://localhost:8000`

---

## Endpoints

### 1. Health Check
Checks the server status.

* **URL**: `/health`
* **Method**: `GET`
* **Auth Required**: No
* **Success Response**:
  * **Code**: `200 OK`
  * **Content**:
    ```json
    {
      "status": "healthy",
      "timestamp": 1690558641.12,
      "version": "1.0.0"
    }
    ```

---

### 2. System Metrics
Exposes performance stats of the deployed Random Forest classifier.

* **URL**: `/metrics`
* **Method**: `GET`
* **Success Response**:
  * **Code**: `200 OK`
  * **Content**:
    ```json
    {
      "dataset": "PhiUSIIL Phishing URL Dataset",
      "model_type": "Random Forest Classifier",
      "features_count": 41,
      "accuracy": 0.999873,
      "precision": 0.999815,
      "recall": 0.999963,
      "f1_score": 0.999889,
      "roc_auc": 1.0
    }
    ```

---

### 3. URL Prediction & Explainability
Crawl page content and analyze URL to check for phishing threat. Calculates a 0-100 risk score and returns localized SHAP explanation reasons.

* **URL**: `/predict`
* **Method**: `POST`
* **Headers**: `Content-Type: application/json`
* **Request Body**:
  ```json
  {
    "url": "https://secure-login-paypal-signin.com"
  }
  ```
* **Success Response**:
  * **Code**: `200 OK`
  * **Content**:
    ```json
    {
      "prediction": "Phishing",
      "confidence": 94.25,
      "risk_score": 88,
      "risk_level": "High Risk",
      "reasons": [
        "HTTPS is disabled (unsecure connection).",
        "Suspicious keywords detected in page url/title.",
        "Webpage contains minimal source code.",
        "Hidden inputs/fields detected in forms."
      ]
    }
    ```

---

### 4. Feature Extraction
Extract 41 features from a live URL and its DOM structure without running model prediction.

* **URL**: `/extract-features`
* **Method**: `POST`
* **Headers**: `Content-Type: application/json`
* **Request Body**:
  ```json
  {
    "url": "https://example.com"
  }
  ```
* **Success Response**:
  * **Code**: `200 OK`
  * **Content**:
    ```json
    {
      "url": "https://example.com",
      "features": {
        "URLLength": 19,
        "DomainLength": 11,
        "IsDomainIP": 0,
        "NoOfSubDomain": 0,
        "NoOfLettersInURL": 15,
        "LetterRatioInURL": 0.789,
        "NoOfDegitsInURL": 0,
        "DegitRatioInURL": 0.0,
        "NoOfEqualsInURL": 0,
        "NoOfQMarkInURL": 0,
        "NoOfAmpersandInURL": 0,
        "NoOfOtherSpecialCharsInURL": 4,
        "SpacialCharRatioInURL": 0.21,
        "IsHTTPS": 1,
        "LineOfCode": 32,
        "LargestLineLength": 110,
        "HasTitle": 1,
        "DomainTitleMatchScore": 1,
        "URLTitleMatchScore": 1,
        "HasFavicon": 1,
        "IsResponsive": 1,
        "NoOfURLRedirect": 0,
        "NoOfSelfRedirect": 2,
        "HasDescription": 1,
        "NoOfPopup": 0,
        "NoOfiFrame": 0,
        "HasExternalFormSubmit": 0,
        "HasSocialNet": 0,
        "HasSubmitButton": 0,
        "HasHiddenFields": 0,
        "HasPasswordField": 0,
        "Bank": 0,
        "Pay": 0,
        "Crypto": 0,
        "HasCopyrightInfo": 1,
        "NoOfImage": 0,
        "NoOfCSS": 1,
        "NoOfJS": 0,
        "NoOfSelfRef": 4,
        "NoOfEmptyRef": 0,
        "NoOfExternalRef": 0
      }
    }
    ```
