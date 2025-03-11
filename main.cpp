#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>

// Function prototypes
void scrapeProxies();
void selectCountry();
void redirectToGitHub();
std::string getDiscordWebhook();
void testProxySpeed(const std::string& proxy);
void displayProxyInfo(const std::string& proxy, const std::string& country, const std::string& source, double speed, bool valid);
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
bool isValidProxy(const std::string& proxy);
double getProxySpeed(const std::string& proxy);
void sendToDiscord(const std::string& discordWebhook, const std::string& message);

int main() {
    int option;

    std::cout << "1. Scrape Proxies\n";
    std::cout << "2. Select Country\n";
    std::cout << "3. GitHub Link\n";
    std::cout << "Select an option: ";
    std::cin >> option;

    switch (option) {
        case 1:
            scrapeProxies();
            break;
        case 2:
            selectCountry();
            break;
        case 3:
            redirectToGitHub();
            break;
        default:
            std::cout << "Invalid option. Exiting...\n";
    }

    return 0;
}

void scrapeProxies() {
    std::string discordWebhook = getDiscordWebhook();
    std::cout << "Here's how the information will be shown:\n";
    std::cout << "[Proxy] [Country] [Source/Link] [Speed] [Status]\n";

    // Example proxy scraping process (use an actual scraping logic for production)
    std::vector<std::string> proxies = {"123.45.67.89:8080", "98.76.54.32:3128"};
    for (const auto& proxy : proxies) {
        std::string country = "US"; // Placeholder for country data
        std::string source = "proxy-source.com"; // Placeholder for source
        double speed = getProxySpeed(proxy);
        bool valid = isValidProxy(proxy);
        displayProxyInfo(proxy, country, source, speed, valid);

        if (valid) {
            sendToDiscord(discordWebhook, "Proxy: " + proxy + " is valid.");
        } else {
            sendToDiscord(discordWebhook, "Proxy: " + proxy + " is invalid.");
        }
    }
}

void selectCountry() {
    std::string country, discordWebhook;
    std::cout << "Enter the country to filter proxies (e.g., US, IN, CA): ";
    std::cin >> country;
    discordWebhook = getDiscordWebhook();

    std::vector<std::string> proxies = {"123.45.67.89:8080", "98.76.54.32:3128"}; 
    for (const auto& proxy : proxies) {
        double speed = getProxySpeed(proxy);
        bool valid = isValidProxy(proxy);
        displayProxyInfo(proxy, country, "proxy-source.com", speed, valid);

        if (valid) {
            sendToDiscord(discordWebhook, "Proxy: " + proxy + " is valid.");
        } else {
            sendToDiscord(discordWebhook, "Proxy: " + proxy + " is invalid.");
        }
    }
}

void redirectToGitHub() {
    std::cout << "Redirecting to GitHub...\n";
    std::cout << "Visit: https://github.com/lukecodingg/proxy-scraper\n";
}

std::string getDiscordWebhook() {
    std::string webhook;
    std::cout << "Enter your Discord Webhook URL: ";
    std::cin >> webhook;
    return webhook;
}

void testProxySpeed(const std::string& proxy) {
    // Proxy speed test logic (mocking speed)
    std::cout << "Testing speed for proxy " << proxy << "...\n";
}

void displayProxyInfo(const std::string& proxy, const std::string& country, const std::string& source, double speed, bool valid) {
    std::string statusColor = valid ? "\033[1;32mValid\033[0m" : "\033[1;31mInvalid\033[0m";
    std::cout << proxy << " [" << country << "] [" << source << "] [" << speed << " ms] [" << statusColor << "]\n";
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool isValidProxy(const std::string& proxy) {
   // WRITE PROXY TESTING
  return true;
}

double getProxySpeed(const std::string& proxy) {
    // Write proxyspeed
    return 100.0; // Speed
}

void sendToDiscord(const std::string& discordWebhook, const std::string& message) {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        Json::Value root;
        root["content"] = message;

        Json::StreamWriterBuilder writer;
        std::string jsonMessage = Json::writeString(writer, root);

        curl_easy_setopt(curl, CURLOPT_URL, discordWebhook.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonMessage.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error sending to Discord: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
}
