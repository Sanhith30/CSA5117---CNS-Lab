import re
from urllib.parse import urlparse
import httpx
from bs4 import BeautifulSoup
from difflib import SequenceMatcher

def is_ip(domain: str) -> int:
    ipv4_pattern = r"^(?:[0-9]{1,3}\.){3}[0-9]{1,3}$"
    ipv6_pattern = r"^s*((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d)|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d)|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4}){0,1}:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]d|1dd|[1-9]?d)\.){3}(25[0-5]|2[0-4]d|1dd|[1-9]?d))|:)))(%.+)?s*$"
    if re.match(ipv4_pattern, domain) or re.match(ipv6_pattern, domain):
        return 1
    return 0

def get_tld(domain: str) -> str:
    parts = domain.split('.')
    if len(parts) > 1:
        return parts[-1]
    return ""

def get_tld_prob(tld: str) -> float:
    tld = tld.lower()
    legitimate_tlds = {'com', 'org', 'net', 'edu', 'gov', 'mil', 'int', 'uk', 'ca', 'au', 'jp', 'de', 'in', 'co'}
    suspicious_tlds = {'xyz', 'top', 'loan', 'club', 'work', 'gq', 'cf', 'tk', 'ml', 'fit', 'buzz', 'bid', 'click', 'science'}
    if tld in legitimate_tlds:
        return 0.98
    elif tld in suspicious_tlds:
        return 0.20
    return 0.50

def get_char_continuation_rate(url: str) -> float:
    if not url:
        return 0.0
    runs = []
    current_type = None
    current_run = 0
    for char in url:
        if char.isalpha():
            char_type = 'l'
        elif char.isdigit():
            char_type = 'd'
        else:
            char_type = 's'
        if char_type == current_type:
            current_run += 1
        else:
            if current_run > 0:
                runs.append(current_run)
            current_type = char_type
            current_run = 1
    if current_run > 0:
        runs.append(current_run)
    return sum(runs) / len(runs) if runs else 0.0

def get_url_char_prob(url: str) -> float:
    if not url:
        return 0.0
    freqs = {
        'e': 0.12, 't': 0.09, 'a': 0.08, 'o': 0.07, 'i': 0.07, 'n': 0.07, 's': 0.06, 'r': 0.06, 'h': 0.06,
        'd': 0.04, 'l': 0.04, 'u': 0.03, 'c': 0.03, 'm': 0.03, 'f': 0.02, 'y': 0.02, 'w': 0.02, 'g': 0.02,
        'p': 0.02, 'b': 0.01, 'v': 0.01, 'k': 0.01, 'x': 0.005, 'q': 0.005, 'j': 0.005, 'z': 0.005
    }
    total = 0.0
    for c in url.lower():
        total += freqs.get(c, 0.001)
    return total / len(url)

def get_similarity_index(domain: str, title: str) -> float:
    if not title:
        return 0.0
    clean_domain = domain.replace("www.", "").split('.')[0]
    return SequenceMatcher(None, clean_domain.lower(), title.lower()).ratio() * 100

def extract_features(url: str) -> dict:
    if not url.startswith(("http://", "https://")):
        url = "https://" + url

    features = {}
    
    # 1. Lexical features (URL string analysis)
    parsed_url = urlparse(url)
    domain = parsed_url.netloc
    tld = get_tld(domain)
    
    features['URLLength'] = len(url)
    features['DomainLength'] = len(domain)
    features['IsDomainIP'] = is_ip(domain)
    
    # Subdomains count
    domain_parts = domain.split('.')
    if features['IsDomainIP'] == 1:
        features['NoOfSubDomain'] = 0
    else:
        features['NoOfSubDomain'] = max(0, len(domain_parts) - 2)
        
    features['NoOfLettersInURL'] = sum(c.isalpha() for c in url)
    features['LetterRatioInURL'] = features['NoOfLettersInURL'] / features['URLLength'] if features['URLLength'] > 0 else 0
    features['NoOfDegitsInURL'] = sum(c.isdigit() for c in url)
    features['DegitRatioInURL'] = features['NoOfDegitsInURL'] / features['URLLength'] if features['URLLength'] > 0 else 0
    features['NoOfEqualsInURL'] = url.count('=')
    features['NoOfQMarkInURL'] = url.count('?')
    features['NoOfAmpersandInURL'] = url.count('&')
    
    special_chars = url.replace('?', '').replace('=', '').replace('&', '')
    features['NoOfOtherSpecialCharsInURL'] = sum(not c.isalnum() for c in special_chars)
    
    total_special = sum(not c.isalnum() for c in url)
    features['SpacialCharRatioInURL'] = total_special / features['URLLength'] if features['URLLength'] > 0 else 0
    features['IsHTTPS'] = 1 if url.startswith("https://") else 0

    # 9 Additional Notebook features (Lexical)
    features['TLDLength'] = len(tld)
    features['TLDLegitimateProb'] = get_tld_prob(tld)
    features['CharContinuationRate'] = get_char_continuation_rate(url)
    features['URLCharProb'] = get_url_char_prob(url)
    features['HasObfuscation'] = 1 if "%" in url else 0
    features['NoOfObfuscatedChar'] = url.count('%')
    features['ObfuscationRatio'] = features['NoOfObfuscatedChar'] / features['URLLength'] if features['URLLength'] > 0 else 0

    # 2. HTML DOM features - Setup defaults in case request fails
    html_content = ""
    redirects_count = 0
    
    try:
        headers = {
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36"
        }
        with httpx.Client(headers=headers, follow_redirects=True, timeout=5.0) as client:
            response = client.get(url)
            html_content = response.text
            redirects_count = len(response.history)
    except Exception as e:
        print(f"Crawling failed for {url}: {str(e)}")
        html_content = ""
        redirects_count = 0

    soup = BeautifulSoup(html_content, "html.parser")
    
    # Compute LineOfCode and LargestLineLength
    lines = html_content.splitlines() if html_content else []
    features['LineOfCode'] = len(lines)
    features['LargestLineLength'] = max(len(line) for line in lines) if lines else 0
    
    # Title analysis
    title_tag = soup.find("title")
    title_text = title_tag.string.strip() if title_tag and title_tag.string else ""
    features['HasTitle'] = 1 if title_text else 0
    
    # Domain/URL Title matches
    clean_domain = domain.replace("www.", "").split('.')[0]
    features['DomainTitleMatchScore'] = 1 if clean_domain and clean_domain.lower() in title_text.lower() else 0
    features['URLTitleMatchScore'] = 1 if title_text and (title_text.lower() in url.lower() or url.lower() in title_text.lower()) else 0
    
    # 9 Additional Notebook features (HTML/DOM)
    features['URLSimilarityIndex'] = get_similarity_index(domain, title_text)
    features['Robots'] = 1 if html_content else 0  # 1 if crawled successfully, 0 otherwise
    
    # Favicon and Responsive check
    favicon = soup.find("link", rel=re.compile(r"^(shortcut |)icon$", re.I))
    features['HasFavicon'] = 1 if favicon else 0
    
    viewport = soup.find("meta", attrs={"name": "viewport"})
    features['IsResponsive'] = 1 if viewport else 0
    
    # Redirects
    features['NoOfURLRedirect'] = redirects_count
    
    # Self redirects count
    all_links = soup.find_all("a", href=True)
    self_redirect_links = 0
    for link in all_links:
        href = link['href']
        if href.startswith(url) or (href.startswith("/") and not href.startswith("//")):
            self_redirect_links += 1
    features['NoOfSelfRedirect'] = self_redirect_links
    
    # Meta description check
    description = soup.find("meta", attrs={"name": "description"})
    features['HasDescription'] = 1 if description else 0
    
    # Popups script check
    scripts_text = "".join(script.string for script in soup.find_all("script") if script.string)
    features['NoOfPopup'] = 1 if "window.open" in scripts_text or "alert(" in scripts_text else 0
    
    features['NoOfiFrame'] = len(soup.find_all("iframe"))
    
    # External Form Submits check
    forms = soup.find_all("form", action=True)
    external_form = 0
    for form in forms:
        action = form['action']
        if action.startswith("http") and not action.startswith(parsed_url.scheme + "://" + domain):
            external_form += 1
    features['HasExternalFormSubmit'] = 1 if external_form > 0 else 0
    
    # Social Network links check
    social_domains = ["facebook.com", "twitter.com", "linkedin.com", "youtube.com", "instagram.com"]
    has_social = 0
    for link in all_links:
        href = link['href']
        if any(sd in href for sd in social_domains):
            has_social = 1
            break
    features['HasSocialNet'] = has_social
    
    # Submit button check
    submit_input = soup.find("input", type="submit")
    submit_button = soup.find("button", type="submit")
    features['HasSubmitButton'] = 1 if (submit_input or submit_button) else 0
    
    # Hidden and Password fields check
    features['HasHiddenFields'] = 1 if soup.find("input", type="hidden") else 0
    features['HasPasswordField'] = 1 if soup.find("input", type="password") else 0
    
    # Word checks (case-insensitive)
    page_text = soup.get_text().lower()
    features['Bank'] = 1 if "bank" in url.lower() or "bank" in page_text else 0
    features['Pay'] = 1 if any(word in url.lower() or word in page_text for word in ["pay", "paypal", "stripe", "checkout", "billing"]) else 0
    features['Crypto'] = 1 if any(word in url.lower() or word in page_text for word in ["crypto", "bitcoin", "btc", "wallet", "ethereum", "eth"]) else 0
    
    # Copyright info check
    features['HasCopyrightInfo'] = 1 if any(word in page_text for word in ["copyright", "©", "(c)", "all rights reserved"]) else 0
    
    # Image/CSS/JS counts
    features['NoOfImage'] = len(soup.find_all("img"))
    features['NoOfCSS'] = len(soup.find_all("link", rel="stylesheet")) + len(soup.find_all("style"))
    features['NoOfJS'] = len(soup.find_all("script"))
    
    # Ref counts
    self_refs = 0
    empty_refs = 0
    external_refs = 0
    for link in all_links:
        href = link['href'].strip()
        if href == "" or href == "#" or href.startswith("javascript:"):
            empty_refs += 1
        elif href.startswith("/") or href.startswith(parsed_url.scheme + "://" + domain) or not href.startswith("http"):
            self_refs += 1
        else:
            external_refs += 1
            
    features['NoOfSelfRef'] = self_refs
    features['NoOfEmptyRef'] = empty_refs
    features['NoOfExternalRef'] = external_refs
    
    return features
