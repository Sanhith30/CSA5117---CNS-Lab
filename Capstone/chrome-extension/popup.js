const BACKEND_URL = "http://localhost:8000";

document.addEventListener("DOMContentLoaded", () => {
  const urlBox = document.getElementById("url-box");
  const scanBtn = document.getElementById("scan-btn");
  const loader = document.getElementById("loader");
  const resultBox = document.getElementById("result-box");
  const threatBadge = document.getElementById("threat-badge");
  const threatScore = document.getElementById("threat-score");
  const confidenceText = document.getElementById("confidence-text");
  const reasonsList = document.getElementById("reasons-list");

  let currentTabUrl = "";

  // 1. Get current active tab URL
  chrome.tabs.query({ active: true, currentWindow: true }, (tabs) => {
    if (tabs && tabs[0]) {
      currentTabUrl = tabs[0].url;
      urlBox.textContent = currentTabUrl;
      
      // If URL is not standard web link (e.g. chrome:// or extensions), disable scanner
      if (!currentTabUrl.startsWith("http://") && !currentTabUrl.startsWith("https://")) {
        urlBox.textContent = "Cannot scan this page type (requires standard HTTP/HTTPS site).";
        scanBtn.disabled = true;
      }
    } else {
      urlBox.textContent = "Unable to detect active tab URL.";
      scanBtn.disabled = true;
    }
  });

  // 2. Scan button click listener
  scanBtn.addEventListener("click", () => {
    if (!currentTabUrl) return;

    scanBtn.disabled = true;
    loader.style.display = "block";
    resultBox.style.display = "none";

    fetch(`${BACKEND_URL}/predict`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({ url: currentTabUrl })
    })
      .then((response) => {
        if (!response.ok) {
          throw new Error("PhishShield API connection error.");
        }
        return response.json();
      })
      .then((data) => {
        // Update popup UI elements
        threatBadge.textContent = data.prediction;
        
        let color = "var(--color-safe)";
        if (data.risk_score > 20 && data.risk_score <= 60) {
          color = "var(--color-suspicious)";
        } else if (data.risk_score > 60) {
          color = "var(--color-phishing)";
        }
        
        threatBadge.style.backgroundColor = color;
        threatScore.textContent = `${data.risk_score} / 100`;
        threatScore.style.color = color;
        confidenceText.textContent = `Confidence: ${data.confidence}% (${data.risk_level})`;

        // Populate reasons
        reasonsList.innerHTML = "";
        data.reasons.forEach((reason) => {
          const li = document.createElement("li");
          li.textContent = reason;
          reasonsList.appendChild(li);
        });

        resultBox.style.display = "block";
        
        // Send threat information to background worker to update badge state
        chrome.runtime.sendMessage({
          action: "updateBadge",
          url: currentTabUrl,
          score: data.risk_score,
          prediction: data.prediction
        });
      })
      .catch((err) => {
        console.error(err);
        urlBox.textContent = "Error: Backend server offline. Please make sure FastAPI backend is running on http://localhost:8000.";
      })
      .finally(() => {
        scanBtn.disabled = false;
        loader.style.display = "none";
      });
  });
});
