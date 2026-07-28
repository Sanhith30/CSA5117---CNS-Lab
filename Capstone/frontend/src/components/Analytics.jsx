import React from 'react';

function Analytics() {
  const modelMetrics = [
    { name: "Random Forest (Optimized)", acc: 99.9873, pre: 99.9815, rec: 99.9963, f1: 99.9889, auc: 100.0 },
    { name: "Decision Tree", acc: 100.0, pre: 100.0, rec: 100.0, f1: 100.0, auc: 100.0 },
    { name: "XGBoost", acc: 100.0, pre: 100.0, rec: 100.0, f1: 100.0, auc: 100.0 },
    { name: "Logistic Regression", acc: 99.9873, pre: 99.9778, rec: 100.0, f1: 99.9889, auc: 100.0 },
    { name: "SVM", acc: 99.9703, pre: 99.9481, rec: 100.0, f1: 99.9741, auc: 100.0 }
  ];

  const topCorrelations = [
    { name: "URLSimilarityIndex", value: 0.86, type: "Positive Correlation" },
    { name: "HasSocialNet", value: 0.78, type: "Positive Correlation" },
    { name: "HasCopyrightInfo", value: 0.74, type: "Positive Correlation" },
    { name: "HasDescription", value: 0.69, type: "Positive Correlation" },
    { name: "IsHTTPS", value: 0.60, type: "Positive Correlation" },
    { name: "SpacialCharRatioInURL", value: -0.53, type: "Negative Correlation" },
    { name: "DegitRatioInURL", value: -0.43, type: "Negative Correlation" },
    { name: "NoOfOtherSpecialCharsInURL", value: -0.35, type: "Negative Correlation" }
  ];

  return (
    <div style={{ display: 'grid', gridTemplateColumns: '1fr', gap: '40px' }}>
      
      {/* Overview stats */}
      <section style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(220px, 1fr))', gap: '20px' }}>
        <div className="glass-card" style={{ textAlign: 'center' }}>
          <h4 style={{ fontSize: '0.85rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.05em', marginBottom: '8px' }}>Dataset Instances</h4>
          <span style={{ fontSize: '2.4rem', fontWeight: 800, color: 'var(--accent)' }}>235,795</span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '4px' }}>UCI PhiUSIIL Phishing URL Corpus</p>
        </div>
        <div className="glass-card" style={{ textAlign: 'center' }}>
          <h4 style={{ fontSize: '0.85rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.05em', marginBottom: '8px' }}>Legitimate Class</h4>
          <span style={{ fontSize: '2.4rem', fontWeight: 800, color: 'var(--color-safe)' }}>134,850</span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '4px' }}>Balanced classification (57.2%)</p>
        </div>
        <div className="glass-card" style={{ textAlign: 'center' }}>
          <h4 style={{ fontSize: '0.85rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.05em', marginBottom: '8px' }}>Phishing Class</h4>
          <span style={{ fontSize: '2.4rem', fontWeight: 800, color: 'var(--color-phishing)' }}>100,945</span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '4px' }}>Target instances (42.8%)</p>
        </div>
        <div className="glass-card" style={{ textAlign: 'center' }}>
          <h4 style={{ fontSize: '0.85rem', color: 'var(--text-secondary)', textTransform: 'uppercase', letterSpacing: '0.05em', marginBottom: '8px' }}>Deployment Features</h4>
          <span style={{ fontSize: '2.4rem', fontWeight: 800, color: 'var(--color-suspicious)' }}>41 / 56</span>
          <p style={{ fontSize: '0.8rem', color: 'var(--text-muted)', marginTop: '4px' }}>Extracted in real-time from active tabs</p>
        </div>
      </section>

      {/* Model comparison */}
      <section className="glass-card">
        <h3 style={{ fontSize: '1.4rem', marginBottom: '24px', fontWeight: 600 }}>Machine Learning Model Comparison</h3>
        
        <div style={{ display: 'grid', gridTemplateColumns: '1fr', gap: '20px' }}>
          {modelMetrics.map((model, idx) => (
            <div key={idx} style={{
              padding: '20px',
              borderRadius: '12px',
              background: 'rgba(255,255,255,0.02)',
              border: '1px solid var(--border-color)',
              display: 'flex',
              flexDirection: 'column',
              gap: '15px'
            }}>
              <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center', flexWrap: 'wrap', gap: '10px' }}>
                <h4 style={{ fontSize: '1.1rem', fontWeight: 600 }}>{model.name}</h4>
                <div style={{ display: 'flex', gap: '10px' }}>
                  <span style={{ background: 'var(--accent-glow)', border: '1px solid var(--accent)', color: 'var(--text-primary)', padding: '2px 8px', borderRadius: '4px', fontSize: '0.8rem', fontWeight: 'bold' }}>
                    Accuracy: {model.acc.toFixed(2)}%
                  </span>
                  <span style={{ background: 'rgba(16,185,129,0.1)', border: '1px solid var(--color-safe)', color: 'var(--color-safe)', padding: '2px 8px', borderRadius: '4px', fontSize: '0.8rem', fontWeight: 'bold' }}>
                    AUC: {model.auc.toFixed(1)}%
                  </span>
                </div>
              </div>

              {/* Progress bars showing metrics */}
              <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))', gap: '15px' }}>
                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem', color: 'var(--text-secondary)', marginBottom: '4px' }}>
                    <span>Precision</span>
                    <strong>{model.pre.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '6px', background: 'rgba(255,255,255,0.05)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.pre}%`, height: '100%', background: 'var(--accent)', borderRadius: '10px' }}></div>
                  </div>
                </div>
                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem', color: 'var(--text-secondary)', marginBottom: '4px' }}>
                    <span>Recall (Detection Rate)</span>
                    <strong>{model.rec.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '6px', background: 'rgba(255,255,255,0.05)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.rec}%`, height: '100%', background: 'var(--color-safe)', borderRadius: '10px' }}></div>
                  </div>
                </div>
                <div>
                  <div style={{ display: 'flex', justifyContent: 'space-between', fontSize: '0.8rem', color: 'var(--text-secondary)', marginBottom: '4px' }}>
                    <span>F1 Score</span>
                    <strong>{model.f1.toFixed(2)}%</strong>
                  </div>
                  <div style={{ height: '6px', background: 'rgba(255,255,255,0.05)', borderRadius: '10px', overflow: 'hidden' }}>
                    <div style={{ width: `${model.f1}%`, height: '100%', background: 'var(--color-suspicious)', borderRadius: '10px' }}></div>
                  </div>
                </div>
              </div>
            </div>
          ))}
        </div>
      </section>

      {/* Feature Correlations */}
      <section style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fit, minmax(320px, 1fr))', gap: '30px' }}>
        
        {/* Correlations card */}
        <div className="glass-card">
          <h3 style={{ fontSize: '1.2rem', marginBottom: '20px' }}>Top Feature Correlations with Target (Label)</h3>
          <div style={{ display: 'flex', flexDirection: 'column', gap: '12px' }}>
            {topCorrelations.map((item, idx) => (
              <div key={idx} style={{
                display: 'flex',
                justifyContent: 'space-between',
                alignItems: 'center',
                padding: '10px 14px',
                borderRadius: '8px',
                background: 'rgba(255,255,255,0.01)',
                border: '1px solid var(--border-color)'
              }}>
                <div>
                  <h5 style={{ fontSize: '0.9rem', color: 'var(--text-primary)' }}>{item.name}</h5>
                  <span style={{ fontSize: '0.75rem', color: item.value > 0 ? 'var(--color-safe)' : 'var(--color-phishing)' }}>
                    {item.type}
                  </span>
                </div>
                <strong style={{ fontSize: '1.05rem', color: 'var(--text-primary)' }}>
                  {item.value > 0 ? `+${item.value}` : item.value}
                </strong>
              </div>
            ))}
          </div>
        </div>

        {/* Explainability / SHAP documentation card */}
        <div className="glass-card" style={{ display: 'flex', flexDirection: 'column', justifyContent: 'space-between' }}>
          <div>
            <h3 style={{ fontSize: '1.2rem', marginBottom: '16px' }}>Explainable AI Design (SHAP)</h3>
            <p style={{ color: 'var(--text-secondary)', fontSize: '0.9rem', lineHeight: 1.6, marginBottom: '15px' }}>
              Unlike black-box models which only offer labels, our platform integrates <strong>SHAP (Shapley Additive exPlanations)</strong>. 
              SHAP calculates the contribution of each extracted webpage attribute to the final classification:
            </p>
            <ul style={{ paddingLeft: '20px', color: 'var(--text-secondary)', fontSize: '0.9rem', lineHeight: 1.6, display: 'flex', flexDirection: 'column', gap: '8px' }}>
              <li><strong>Positive SHAP</strong> pushes predictions towards "Phishing" (e.g. absent HTTPS certificate, hidden input fields).</li>
              <li><strong>Negative SHAP</strong> pushes predictions towards "Legitimate" (e.g. extensive code structure, domain-title matches).</li>
              <li>Values are parsed at runtime to extract top diagnostic factors and formulate clear warnings.</li>
            </ul>
          </div>
          
          <div style={{ marginTop: '20px', padding: '16px', background: 'var(--accent-glow)', border: '1px solid var(--accent)', borderRadius: '10px' }}>
            <h5 style={{ color: '#fff', fontSize: '0.9rem', marginBottom: '6px' }}>Performance Note</h5>
            <p style={{ color: 'var(--text-secondary)', fontSize: '0.8rem', lineHeight: 1.4 }}>
              The deployment model uses 41 features, retaining <strong>99.98% accuracy</strong> compared to the 50-feature research model. 
              This guarantees enterprise-level security for active browser extension scans.
            </p>
          </div>
        </div>

      </section>

    </div>
  );
}

export default Analytics;
