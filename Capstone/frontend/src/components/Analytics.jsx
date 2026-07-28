import React, { useState } from 'react';

function Analytics() {
  const [activeTooltip, setActiveTooltip] = useState(null);

  const modelMetrics = [
    { name: "Random Forest (Optimized)", acc: 99.98, pre: 99.98, rec: 99.99, f1: 99.98, auc: 100.0, desc: "Primary deployment model optimized for live crawler feature subsets." },
    { name: "XGBoost Classifier", acc: 100.0, pre: 100.0, rec: 100.0, f1: 100.0, auc: 100.0, desc: "High boosting model showing absolute linear separation on training sets." },
    { name: "Decision Tree", acc: 99.95, pre: 99.94, rec: 99.96, f1: 99.95, auc: 99.96, desc: "Single tree configuration used for fast baseline evaluations." },
    { name: "Logistic Regression", acc: 99.92, pre: 99.90, rec: 99.94, f1: 99.92, auc: 99.95, desc: "Standard linear baseline classifier fit on normalized scalar profiles." },
    { name: "Support Vector Machine", acc: 99.97, pre: 99.94, rec: 100.0, f1: 99.97, auc: 100.0, desc: "Kernelized boundary separator mapping high-dimensional profiles." }
  ];

  const topCorrelations = [
    { name: "URLSimilarityIndex", value: 0.86, type: "Positive Correlation", desc: "Measures domain lexical similarity ratio against webpage body keywords." },
    { name: "HasSocialNet", value: 0.78, type: "Positive Correlation", desc: "Flags presence of legitimate social network connectivity anchors." },
    { name: "HasCopyrightInfo", value: 0.74, type: "Positive Correlation", desc: "Detects legal ownership assertions in footer structures." },
    { name: "HasDescription", value: 0.69, type: "Positive Correlation", desc: "Verifies standard metadata descriptors in headers." },
    { name: "IsHTTPS", value: 0.60, type: "Positive Correlation", desc: "Validates secure TLS encryption wrapping." },
    { name: "SpacialCharRatioInURL", value: -0.53, type: "Negative Correlation", desc: "Measures URL string character distributions (high special character ratios indicate obfuscation)." },
    { name: "DegitRatioInURL", value: -0.43, type: "Negative Correlation", desc: "Checks numeric content density inside domains (phishing domains often use random digit strings)." }
  ];

  return (
    <div style={{ display: 'flex', flexDirection: 'column', gap: '40px' }}>
      
      {/* Overview Stat Widgets */}
      <section style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '24px' }}>
        
        <div className="cyber-card" style={{ textAlign: 'center', borderTop: '3px solid var(--accent-primary)' }}>
          <h4 style={{ fontSize: '0.8rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.08em', marginBottom: '12px' }}>
            Dataset Instances
          </h4>
          <span style={{ fontSize: '2.6rem', fontWeight: 800, fontFamily: 'var(--font-heading)', background: 'var(--accent-gradient)', WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent' }}>
            235,795
          </span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '8px' }}>
            UCI PhiUSIIL Phishing URL Corpus
          </p>
        </div>

        <div className="cyber-card" style={{ textAlign: 'center', borderTop: '3px solid var(--color-safe)' }}>
          <h4 style={{ fontSize: '0.8rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.08em', marginBottom: '12px' }}>
            Legitimate Samples
          </h4>
          <span style={{ fontSize: '2.6rem', fontWeight: 800, fontFamily: 'var(--font-heading)', color: 'var(--color-safe)' }}>
            134,850
          </span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '8px' }}>
            Balanced Class Ratio (57.2%)
          </p>
        </div>

        <div className="cyber-card" style={{ textAlign: 'center', borderTop: '3px solid var(--color-phishing)' }}>
          <h4 style={{ fontSize: '0.8rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.08em', marginBottom: '12px' }}>
            Phishing Samples
          </h4>
          <span style={{ fontSize: '2.6rem', fontWeight: 800, fontFamily: 'var(--font-heading)', color: 'var(--color-phishing)' }}>
            100,945
          </span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '8px' }}>
            Target Class Ratio (42.8%)
          </p>
        </div>

        <div className="cyber-card" style={{ textAlign: 'center', borderTop: '3px solid var(--color-suspicious)' }}>
          <h4 style={{ fontSize: '0.8rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.08em', marginBottom: '12px' }}>
            Active Feature Map
          </h4>
          <span style={{ fontSize: '2.6rem', fontWeight: 800, fontFamily: 'var(--font-heading)', color: 'var(--color-suspicious)' }}>
            50 / 50
          </span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '8px' }}>
            Full dimensional model accuracy
          </p>
        </div>

      </section>

      {/* Model comparative cards */}
      <section className="cyber-card">
        <h3 style={{ fontSize: '1.4rem', fontWeight: 800, marginBottom: '28px', textTransform: 'uppercase', letterSpacing: '-0.01em' }}>
          Machine Learning Model Performance Matrix
        </h3>
        
        <div style={{ display: 'flex', flexDirection: 'column', gap: '20px' }}>
          {modelMetrics.map((model, idx) => (
            <div 
              key={idx} 
              style={{
                padding: '22px 28px',
                borderRadius: '16px',
                background: 'rgba(255, 255, 255, 0.01)',
                border: '1px solid var(--border-color)',
                display: 'flex',
                flexDirection: 'column',
                gap: '16px',
                transition: 'all 0.2s ease'
              }}
              onMouseOver={(e) => e.currentTarget.style.borderColor = 'rgba(99, 102, 241, 0.2)'}
              onMouseOut={(e) => e.currentTarget.style.borderColor = 'var(--border-color)'}
            >
              {/* Header and overview metric */}
              <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', flexWrap: 'wrap', gap: '10px' }}>
                <div>
                  <h4 style={{ fontSize: '1.1rem', fontWeight: 700, color: 'var(--text-primary)' }}>{model.name}</h4>
                  <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '2px' }}>{model.desc}</p>
                </div>
                <div style={{ display: 'flex', gap: '12px' }}>
                  <span style={{ background: 'rgba(99, 102, 241, 0.1)', border: '1px solid var(--accent-primary)', color: 'var(--text-primary)', padding: '4px 10px', borderRadius: '6px', fontSize: '0.78rem', fontWeight: 'bold' }}>
                    ACC: {model.acc.toFixed(2)}%
                  </span>
                  <span style={{ background: 'rgba(16, 185, 129, 0.08)', border: '1px solid var(--color-safe)', color: 'var(--color-safe)', padding: '4px 10px', borderRadius: '6px', fontSize: '0.78rem', fontWeight: 'bold' }}>
                    AUC: {model.auc.toFixed(1)}%
                  </span>
                </div>
              </div>

              {/* Progress sliders */}
              <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '20px', borderTop: '1px solid rgba(255,255,255,0.03)', paddingTop: '16px' }}>
                
                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.78rem', color: 'var(--text-secondary)', marginBottom: '6px' }}>
                    <span>Model Precision</span>
                    <strong>{model.pre.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '5px', background: 'rgba(255,255,255,0.04)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.pre}%`, height: '100%', background: 'var(--accent-gradient)', borderRadius: '10px' }}></div>
                  </div>
                </div>

                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.78rem', color: 'var(--text-secondary)', marginBottom: '6px' }}>
                    <span>Model Recall (Sensivity)</span>
                    <strong>{model.rec.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '5px', background: 'rgba(255,255,255,0.04)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.rec}%`, height: '100%', background: 'var(--color-safe-gradient)', borderRadius: '10px' }}></div>
                  </div>
                </div>

                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.78rem', color: 'var(--text-secondary)', marginBottom: '6px' }}>
                    <span>F1 Score (Harmonic Mean)</span>
                    <strong>{model.f1.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '5px', background: 'rgba(255,255,255,0.04)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.f1}%`, height: '100%', background: 'var(--color-suspicious-gradient)', borderRadius: '10px' }}></div>
                  </div>
                </div>

              </div>
            </div>
          ))}
        </div>
      </section>

      {/* Feature stats and SHAP Details */}
      <section style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(320px, 1fr))', gap: '30px' }} className="responsive-grid">
        
        {/* Core Correlations list with hover card tooltips */}
        <div className="cyber-card">
          <h3 style={{ fontSize: '1.25rem', fontWeight: 700, marginBottom: '20px' }}>Statistical Feature Correlations</h3>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '12px' }}>
            {topCorrelations.map((item, idx) => (
              <div 
                key={idx} 
                style={{
                  display: 'flex',
                  justifyContent: 'space-between',
                  alignItems: 'center',
                  padding: '12px 16px',
                  borderRadius: '10px',
                  background: 'rgba(255,255,255,0.01)',
                  border: '1px solid var(--border-color)',
                  position: 'relative',
                  cursor: 'help'
                }}
                onMouseEnter={() => setActiveTooltip(idx)}
                onMouseLeave={() => setActiveTooltip(null)}
              >
                <div>
                  <h5 style={{ fontSize: '0.9rem', fontWeight: 600, color: 'var(--text-primary)' }}>{item.name}</h5>
                  <span style={{ fontSize: '0.75rem', color: item.value > 0 ? 'var(--color-safe)' : 'var(--color-phishing)' }}>
                    {item.type}
                  </span>
                </div>
                <strong style={{ fontSize: '1rem', color: 'var(--text-primary)', fontFamily: 'var(--font-heading)' }}>
                  {item.value > 0 ? `+${item.value}` : item.value}
                </strong>

                {/* Micro tooltip */}
                {activeTooltip === idx && (
                  <div style={{
                    position: 'absolute',
                    bottom: '100%',
                    left: '50%',
                    transform: 'translate(-50%, -8px)',
                    background: '#1f2937',
                    border: '1px solid rgba(255, 255, 255, 0.08)',
                    borderRadius: '8px',
                    padding: '8px 12px',
                    width: '200px',
                    zIndex: 10,
                    boxShadow: '0 4px 15px rgba(0, 0, 0, 0.3)',
                    color: 'var(--text-primary)',
                    fontSize: '0.75rem',
                    lineHeight: 1.3,
                    pointerEvents: 'none'
                  }}>
                    {item.desc}
                  </div>
                )}
              </div>
            ))}
          </div>
        </div>

        {/* Math explanation SHAP visual */}
        <div className="cyber-card" style={{ display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
          <div>
            <h3 style={{ fontSize: '1.25rem', fontWeight: 700, marginBottom: '18px' }}>Explainable AI Integration (SHAP)</h3>
            <p style={{ color: 'var(--text-secondary)', fontSize: '0.9rem', lineHeight: 1.6, marginBottom: '16px' }}>
              Standard neural network or tree ensemble systems operate as opaque boxes. 
              To secure clinical, banking, or enterprise domains, <strong>PhishShield AI</strong> overlays <strong>SHAP (Shapley Additive exPlanations)</strong>:
            </p>
            <ul style={{ paddingLeft: '20px', color: 'var(--text-secondary)', fontSize: '0.88rem', lineHeight: 1.6, display: 'flex', flexDirection: 'column', gap: '10px' }}>
              <li>
                <strong>Game Theoretic Impact:</strong> Calculates feature contributions by examining all combinations of input features to assign accurate weight values.
              </li>
              <li>
                <strong>Positive Shifts:</strong> Features that drive scores up towards phishing (e.g. ObfuscationRatio, low line counts).
              </li>
              <li>
                <strong>Negative Shifts:</strong> Trusted features that decrease risk scores toward legitimate status (e.g. copyright info, responsive layout structures).
              </li>
            </ul>
          </div>

          <div style={{
            marginTop: '25px',
            padding: '18px',
            background: 'var(--accent-glow)',
            border: '1px solid var(--accent-primary)',
            borderRadius: '14px',
            boxShadow: '0 4px 20px rgba(99, 102, 241, 0.15)'
          }}>
            <h5 style={{ color: '#fff', fontSize: '0.92rem', fontWeight: 700, marginBottom: '6px' }}>Model Deployment Integrity</h5>
            <p style={{ color: 'var(--text-secondary)', fontSize: '0.8rem', lineHeight: 1.45 }}>
              The Random Forest classifier deployed in this platform achieves a validation F1-score of <strong>99.98%</strong>. 
              SHAP trees resolve explanations in milliseconds, enabling real-time badge warning notifications in browser modules.
            </p>
          </div>
        </div>

      </section>

    </div>
  );
}

export default Analytics;
