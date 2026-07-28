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
      confidence: 100.0,
      risk_score: 0
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

  const totalThreats = scanHistory.filter(s => s.prediction === "Phishing").length;

  return (
    <div style={{ minHeight: '100vh', display: 'flex', flexDirection: 'column', position: 'relative', zIndex: 1 }}>
      {/* Background Orbs */}
      <div className="ambient-glow-1"></div>
      <div className="ambient-glow-2"></div>

      {/* Floating Cyber Glass Header */}
      <header style={{
        background: 'rgba(10, 15, 30, 0.4)',
        backdropFilter: 'blur(20px)',
        WebkitBackdropFilter: 'blur(20px)',
        borderBottom: '1px solid rgba(255, 255, 255, 0.05)',
        position: 'sticky',
        top: 0,
        zIndex: 100,
        padding: '18px 6%',
        boxShadow: '0 4px 30px rgba(0, 0, 0, 0.2)'
      }}>
        <div style={{
          display: 'flex',
          justifyContent: 'space-between',
          alignItems: 'center',
          maxWidth: '1400px',
          margin: '0 auto'
        }}>
          {/* Logo & Platform Info */}
          <div style={{ display: 'flex', alignItems: 'center', gap: '14px' }}>
            <div style={{
              width: '40px',
              height: '40px',
              borderRadius: '12px',
              background: 'linear-gradient(135deg, #6366f1, #a855f7)',
              display: 'flex',
              alignItems: 'center',
              justifyContent: 'center',
              boxShadow: '0 0 20px rgba(99, 102, 241, 0.4)'
            }}>
              {/* Pulsing Shield SVG */}
              <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2.5" strokeLinecap="round" strokeLinejoin="round" style={{ color: '#fff' }}>
                <path d="M12 22s8-4 8-10V5l-8-3-8 3v7c0 6 8 10 8 10z"/>
              </svg>
            </div>
            <div>
              <h1 style={{ 
                fontSize: '1.4rem', 
                fontWeight: 800, 
                letterSpacing: '-0.02em',
                background: 'linear-gradient(to right, #fff, #9ca3af)', 
                WebkitBackgroundClip: 'text', 
                WebkitTextFillColor: 'transparent',
                display: 'flex',
                alignItems: 'center',
                gap: '8px'
              }}>
                PhishShield AI
              </h1>
              <span style={{ fontSize: '0.75rem', color: 'var(--text-muted)', fontWeight: 600, textTransform: 'uppercase', letterSpacing: '0.05em' }}>
                XAI Phishing Intelligence
              </span>
            </div>
          </div>

          {/* Real-time Status Badges */}
          <div style={{ display: 'flex', alignItems: 'center', gap: '20px', flexWrap: 'wrap' }} className="hide-mobile">
            <div style={{
              display: 'flex',
              alignItems: 'center',
              gap: '8px',
              background: 'rgba(255, 255, 255, 0.02)',
              border: '1px solid rgba(255, 255, 255, 0.05)',
              padding: '6px 12px',
              borderRadius: '8px',
              fontSize: '0.8rem'
            }}>
              <span style={{ width: '8px', height: '8px', borderRadius: '50%', background: 'var(--color-safe)', display: 'inline-block', boxShadow: '0 0 8px var(--color-safe)' }}></span>
              <span style={{ color: 'var(--text-secondary)', fontWeight: 500 }}>System Status:</span>
              <strong style={{ color: 'var(--text-primary)' }}>SECURE</strong>
            </div>

            <div style={{
              display: 'flex',
              alignItems: 'center',
              gap: '8px',
              background: 'rgba(239, 68, 68, 0.05)',
              border: '1px solid rgba(239, 68, 68, 0.15)',
              padding: '6px 12px',
              borderRadius: '8px',
              fontSize: '0.8rem'
            }}>
              <span style={{ color: 'var(--color-phishing)', fontWeight: 500 }}>Blocked threats:</span>
              <strong style={{ color: 'var(--color-phishing)' }}>{totalThreats}</strong>
            </div>
          </div>

          {/* Navigation Controls */}
          <nav style={{ 
            display: 'flex', 
            background: 'rgba(0,0,0,0.2)', 
            padding: '4px', 
            borderRadius: '10px', 
            border: '1px solid rgba(255,255,255,0.03)' 
          }}>
            <button
              onClick={() => setActiveTab('dashboard')}
              style={{
                background: activeTab === 'dashboard' ? 'var(--accent-gradient)' : 'transparent',
                color: activeTab === 'dashboard' ? '#fff' : 'var(--text-secondary)',
                border: 'none',
                padding: '8px 20px',
                borderRadius: '8px',
                cursor: 'pointer',
                fontFamily: 'var(--font-heading)',
                fontWeight: 600,
                fontSize: '0.9rem',
                transition: 'all 0.2s ease',
                boxShadow: activeTab === 'dashboard' ? '0 4px 10px rgba(99, 102, 241, 0.2)' : 'none'
              }}
            >
              Console
            </button>
            <button
              onClick={() => setActiveTab('analytics')}
              style={{
                background: activeTab === 'analytics' ? 'var(--accent-gradient)' : 'transparent',
                color: activeTab === 'analytics' ? '#fff' : 'var(--text-secondary)',
                border: 'none',
                padding: '8px 20px',
                borderRadius: '8px',
                cursor: 'pointer',
                fontFamily: 'var(--font-heading)',
                fontWeight: 600,
                fontSize: '0.9rem',
                transition: 'all 0.2s ease',
                boxShadow: activeTab === 'analytics' ? '0 4px 10px rgba(99, 102, 241, 0.2)' : 'none'
              }}
            >
              Analytics
            </button>
          </nav>
        </div>
      </header>

      {/* Main Workspace Frame */}
      <main style={{
        flex: 1,
        padding: '30px 6%',
        maxWidth: '1400px',
        width: '100%',
        margin: '0 auto',
        animation: 'slide-up-fade 0.5s ease-out'
      }}>
        {activeTab === 'dashboard' ? (
          <Dashboard scanHistory={scanHistory} addScanToHistory={addScanToHistory} />
        ) : (
          <Analytics />
        )}
      </main>

      {/* Space Footer */}
      <footer style={{
        background: 'rgba(3, 7, 18, 0.8)',
        borderTop: '1px solid rgba(255, 255, 255, 0.03)',
        padding: '30px 6%',
        color: 'var(--text-muted)',
        fontSize: '0.85rem',
        marginTop: '50px'
      }}>
        <div style={{ 
          maxWidth: '1400px', 
          margin: '0 auto', 
          display: 'flex', 
          justifyContent: 'space-between', 
          alignItems: 'center', 
          flexWrap: 'wrap', 
          gap: '15px' 
        }}>
          <div>
            <p style={{ fontWeight: 600, color: 'var(--text-secondary)' }}>PhishShield AI Dashboard</p>
            <p style={{ fontSize: '0.75rem', marginTop: '2px' }}>Academic Capstone Security Research project</p>
          </div>
          <div style={{ display: 'flex', gap: '24px', fontWeight: 500 }}>
            <span>Powered by FastAPI</span>
            <span>SHAP Explanations</span>
            <span>Random Forest Classifier</span>
          </div>
        </div>
      </footer>
    </div>
  );
}

export default App;
