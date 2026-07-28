import React, { useState } from 'react';
import Dashboard from './components/Dashboard';
import Analytics from './components/Analytics';

function App() {
  const [activeTab, setActiveTab] = useState('dashboard');
  const [scanHistory, setScanHistory] = useState([
    {
      id: "1",
      url: "https://secure-login-paypal.com/signin",
      timestamp: "10 minutes ago",
      prediction: "Phishing",
      confidence: 98.4,
      risk_score: 96
    },
    {
      id: "2",
      url: "https://www.google.com",
      timestamp: "1 hour ago",
      prediction: "Legitimate",
      confidence: 99.8,
      risk_score: 2
    },
    {
      id: "3",
      url: "http://mybank-update-alert.net/login.php",
      timestamp: "2 hours ago",
      prediction: "Phishing",
      confidence: 88.5,
      risk_score: 75
    }
  ]);

  const addScanToHistory = (url, prediction, confidence, risk_score) => {
    const newScan = {
      id: Date.now().toString(),
      url,
      timestamp: "Just now",
      prediction,
      confidence,
      risk_score
    };
    setScanHistory(prev => [newScan, ...prev].slice(0, 10)); // Keep top 10 scans
  };

  return (
    <div style={{ minHeight: '100vh', display: 'flex', flexDirection: 'column' }}>
      {/* Header / Nav */}
      <header style={{
        background: 'rgba(10, 15, 29, 0.7)',
        backdropFilter: 'blur(10px)',
        borderBottom: '1px solid var(--border-color)',
        position: 'sticky',
        top: 0,
        zIndex: 100,
        padding: '16px 5%'
      }}>
        <div style={{
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
          maxWidth: '1400px',
          margin: '0 auto'
        }}>
          {/* Logo */}
          <div style={{ display: 'flex', alignItems: 'center', gap: '10px' }}>
            <div style={{
              width: '32px',
              height: '32px',
              borderRadius: '8px',
              background: 'linear-gradient(135deg, var(--accent), #ff4d4d)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              fontWeight: 'bold',
              fontSize: '1.2rem',
              color: '#fff',
              boxShadow: '0 0 15px rgba(99,102,241,0.5)'
            }}>
              Φ
            </div>
            <h1 style={{ fontSize: '1.3rem', fontWeight: 700, background: 'linear-gradient(to right, #fff, #94a3b8)', WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent' }}>
              PhishShield AI
            </h1>
          </div>

          {/* Navigation tabs */}
          <nav style={{ display: 'flex', gap: '8px' }}>
            <button
              onClick={() => setActiveTab('dashboard')}
              style={{
                background: activeTab === 'dashboard' ? 'rgba(255,255,255,0.05)' : 'transparent',
                color: activeTab === 'dashboard' ? 'var(--text-primary)' : 'var(--text-secondary)',
                border: 'none',
                padding: '8px 16px',
                borderRadius: '8px',
                cursor: 'pointer',
                fontFamily: 'var(--font-heading)',
                fontWeight: 600,
                fontSize: '0.95rem',
                transition: 'var(--transition-smooth)'
              }}
            >
              Scanner Dashboard
            </button>
            <button
              onClick={() => setActiveTab('analytics')}
              style={{
                background: activeTab === 'analytics' ? 'rgba(255,255,255,0.05)' : 'transparent',
                color: activeTab === 'analytics' ? 'var(--text-primary)' : 'var(--text-secondary)',
                border: 'none',
                padding: '8px 16px',
                borderRadius: '8px',
                cursor: 'pointer',
                fontFamily: 'var(--font-heading)',
                fontWeight: 600,
                fontSize: '0.95rem',
                transition: 'var(--transition-smooth)'
              }}
            >
              Model Analytics
            </button>
          </nav>
        </div>
      </header>

      {/* Main Content */}
      <main style={{
        flex: 1,
        padding: '40px 5%',
        maxWidth: '1400px',
        width: '100%',
        margin: '0 auto'
      }}>
        {activeTab === 'dashboard' ? (
          <Dashboard scanHistory={scanHistory} addScanToHistory={addScanToHistory} />
        ) : (
          <Analytics />
        )}
      </main>

      {/* Footer */}
      <footer style={{
        background: 'rgba(10, 15, 29, 0.9)',
        borderTop: '1px solid var(--border-color)',
        padding: '24px 5%',
        textAlign: 'center',
        color: 'var(--text-muted)',
        fontSize: '0.85rem'
      }}>
        <div style={{ maxWidth: '1400px', margin: '0 auto', display: 'flex', justifyContent: 'space-between', alignItems: 'center', flexWrap: 'wrap', gap: '15px' }}>
          <p>© 2026 PhishShield AI Platform. All rights reserved.</p>
          <div style={{ display: 'flex', gap: '20px' }}>
            <span style={{ color: 'var(--text-muted)' }}>Security Research Project</span>
            <span style={{ color: 'var(--text-muted)' }}>Powered by FastAPI & Random Forest</span>
          </div>
        </div>
      </footer>
    </div>
  );
}

export default App;
