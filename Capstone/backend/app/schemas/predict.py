from pydantic import BaseModel, HttpUrl, Field
from typing import List, Dict, Any

class PredictRequest(BaseModel):
    url: str = Field(..., min_length=1, description="The URL of the website to analyze", example="https://example.com")

class PredictResponse(BaseModel):
    prediction: str = Field(..., description="Prediction label: 'Phishing' or 'Legitimate'", example="Phishing")
    confidence: float = Field(..., description="Confidence percentage of the prediction", example=98.2)
    risk_score: int = Field(..., description="Risk score from 0 (very safe) to 100 (critical)", example=96)
    risk_level: str = Field(..., description="Risk level category", example="Critical")
    reasons: List[str] = Field(..., description="Human-readable reasons for the classification")

class ExtractRequest(BaseModel):
    url: str = Field(..., min_length=1, description="The URL to extract features from")

class ExtractResponse(BaseModel):
    url: str
    features: Dict[str, Any] = Field(..., description="Extracted feature names and values")

class PredictFeaturesRequest(BaseModel):
    features: Dict[str, Any] = Field(..., description="Dictionary containing all 50 features and their values")
