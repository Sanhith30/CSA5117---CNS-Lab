// Background Service Worker for Manifest V3 Chrome Extension

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  if (message.action === "updateBadge") {
    const { score, prediction } = message;
    
    let badgeText = "SAFE";
    let badgeColor = "#10b981"; // Green

    if (score > 20 && score <= 60) {
      badgeText = "WARN";
      badgeColor = "#f59e0b"; // Yellow
    } else if (score > 60) {
      badgeText = "RISK";
      badgeColor = "#ef4444"; // Red
    }

    // Update Extension Badge for the active tab context
    chrome.action.setBadgeText({ text: badgeText });
    chrome.action.setBadgeBackgroundColor({ color: badgeColor });
    
    sendResponse({ status: "success" });
  }
  return true; // Keep message channel open for async response
});
