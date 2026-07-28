# Intelligent Phishing Website Detection Platform

An end-to-end cybersecurity solution to detect phishing websites in real-time. The system utilizes machine learning trained on the **PhiUSIIL Phishing URL Dataset** and leverages **SHAP (Shapley Additive exPlanations)** to provide transparent, human-readable reasons for classifications alongside a 0-100 risk score.

---

## Key Features
1. **Real-time Feature Crawler**: Extracts 41 lexical and DOM webpage features (lines of code, forms, password fields, styles, hidden inputs, redirection count) from a live site.
2. **Predictive Model**: REST API served via **FastAPI** using a high-precision **Random Forest Classifier** (99.98% Accuracy, 100.00% ROC-AUC).
3. **Explainable AI (XAI)**: Identifies top risk contributors using local SHAP explanations.
4. **Interactive Dashboard**: A responsive **React** (Vite) interface featuring risk gauges, diagnostic panels, historic checks, and analytics.
5. **Manifest V3 Chrome Extension**: Monitors browser tabs, crawls the site in real-time, displays risk alerts, and updates color-coded indicators (Green = Safe, Yellow = Suspicious, Red = Phishing).

---

## Directory Structure
```text
project/
├── backend/                   # FastAPI backend source code
│   ├── app/
│   │   ├── main.py            # API entrypoint and routes
│   │   ├── schemas/           # Pydantic validation request/response models
│   │   └── services/          # Lexical/DOM Crawling & SHAP Prediction
│   ├── models/                # Trained deployment binaries
│   ├── Dockerfile
│   └── requirements.txt
├── frontend/                  # React dashboard
│   ├── src/
│   │   ├── components/        # Dashboard panels, Analytics charts
│   │   ├── App.jsx            # State nav controller
│   │   └── index.css          # Glassmorphic Slate styling system
│   ├── Dockerfile
│   └── package.json
├── chrome-extension/          # Manifest V3 Chrome Extension source
│   ├── manifest.json          # Extension permissions & entrypoints
│   ├── popup.html             # Diagnostic panel UI
│   ├── popup.js               # Extension AJAX requests
│   └── background.js          # Background badge controller
├── ml/                        # ML Pipeline & notebooks
│   ├── datasets/              # UCI Phishing URL Corpus csv
│   ├── notebooks/             # Research & EDA notebooks
│   └── train_deployment_model.py # Deployable model builder
├── tests/                     # Automated unittest suite
│   ├── test_backend.py        # API route checks
│   └── test_extractor.py      # DOM crawler checks
├── docker-compose.yml         # Container orchestration
└── README.md                  # System setup guide
```

---

## Quick Start (Manual Execution)

### 1. Build Deployable Models
Train the lightweight deployment model using the 41 extractable features:
```bash
python ml/train_deployment_model.py
```

### 2. Start the Backend API
Install dependencies and run the FastAPI server on port 8000:
```bash
cd backend
pip install -r requirements.txt
uvicorn app.main:app --reload --port 8000
```
*API documentation will be available at: `http://localhost:8000/docs`*

### 3. Start the Frontend Dashboard
Install Node dependencies and start Vite dev server:
```bash
cd ../frontend
npm install
npm run dev
```

### 4. Load the Chrome Extension
1. Open Google Chrome and navigate to `chrome://extensions/`.
2. Enable **Developer mode** (top-right toggle).
3. Click **Load unpacked** (top-left button).
4. Select the `chrome-extension/` directory from this workspace.

---

## Docker Deployment (Orchestrated)
Build and run both backend and frontend microservices together:
```bash
docker-compose up --build
```
* The backend is exposed at `http://localhost:8000`
* The React dashboard is exposed at `http://localhost:3000`

---

## Running Tests
Run python unit and integration tests:
```bash
python -m unittest discover -s tests
```
