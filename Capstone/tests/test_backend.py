import unittest
from fastapi.testclient import TestClient
import os
import sys

# Append backend to PYTHONPATH so we can import app
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(__file__)), "backend"))

from app.main import app

class TestBackendAPI(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.client = TestClient(app)

    def test_health_check(self):
        response = self.client.get("/health")
        self.assertEqual(response.status_code, 200)
        json_data = response.json()
        self.assertEqual(json_data["status"], "healthy")
        self.assertIn("version", json_data)

    def test_get_metrics(self):
        response = self.client.get("/metrics")
        self.assertEqual(response.status_code, 200)
        json_data = response.json()
        self.assertEqual(json_data["model_type"], "Random Forest Classifier")
        self.assertIn("accuracy", json_data)
        self.assertIn("f1_score", json_data)

    def test_predict_endpoint_legitimate(self):
        # A standard legitimate URL scan
        response = self.client.post("/predict", json={"url": "https://www.google.com"})
        self.assertEqual(response.status_code, 200)
        json_data = response.json()
        self.assertIn("prediction", json_data)
        self.assertIn("confidence", json_data)
        self.assertIn("risk_score", json_data)
        self.assertIn("reasons", json_data)
        self.assertIsInstance(json_data["reasons"], list)

    def test_predict_endpoint_empty_url(self):
        response = self.client.post("/predict", json={"url": ""})
        self.assertEqual(response.status_code, 422) # validation error on empty string or handled

    def test_extract_features_endpoint(self):
        response = self.client.post("/extract-features", json={"url": "https://example.com"})
        self.assertEqual(response.status_code, 200)
        json_data = response.json()
        self.assertEqual(json_data["url"], "https://example.com")
        self.assertIn("features", json_data)
        self.assertIsInstance(json_data["features"], dict)

if __name__ == "__main__":
    unittest.main()
