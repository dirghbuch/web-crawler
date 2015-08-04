#include <iostream>
#include <iterator>
#include <fstream>
#include <boost/regex.hpp>
#include <curl/curl.h>

static size_t writeToBuffer(void *contents, size_t size, size_t nmemb, void *userdata) {
    ((std::string*)userdata)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
struct returned_array {
    std::string result[100];
};
std::ofstream file;
boost::regex e("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", boost::regex::normal | boost::regbase::icase);
boost::regex m("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", boost::regex::normal | boost::regbase::icase);
class Crawler {
	public:
		std::string base_url;
		std::string html_content;
		std::string current_url;
		int link_count;
		int depth;
		void readHtmlFromUrl( const char* url) {
			CURL *curl;
			CURLcode resource;
			curl = curl_easy_init();
			if(curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToBuffer);
			    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->html_content);
				resource = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
			}
		}

		returned_array extractLinksFromHtml() {
			boost::sregex_token_iterator i(this->html_content.begin(), this->html_content.end(), e, 1);
			boost::sregex_token_iterator j;
			returned_array child_links;
			std::string child_url;
			int count = 0;
			file << std::endl;
			while(i != j) {
				boost::regex re("^(?:http://)?([^/]+)(?:/?.*/?)/(.*)$");
				if ( !boost::regex_match(i->str(), re) ){
				  child_url = this->base_url + '/' + i->str();
				} else {
					child_url =  i->str();
				}
				*i++;
				file << child_url << std::endl;
				child_links.result[count] = child_url;
				count++;
				if( count == this->link_count ){
					return child_links;
				}
			}
		}

		int countNumberOfUniqueWordsFromHtml() {
			std::string word;
			std::istringstream html_string(this->html_content);
		    std::map<std::string, int> word_count;
		    int total_words = 0;

		    while (std::getline(html_string, word, ' ')) {
		        word_count[word]++;
		    }

		    typedef std::map<std::string, int>::iterator iter;
		    iter end = word_count.end();
		    for(iter it = word_count.begin(); it != end; ++it) {
		        total_words += it->second;
		    }

    		return total_words;		
		}
};

int crawlChilds(Crawler crwl, returned_array links, int current_depth){
	current_depth++;
	file << "-------------Level "  << current_depth << "------------------" << std::endl << std::endl;;
	for(int i=0; i < crwl.link_count; i++) {
		file<<"-------------- Link: "<< links.result[i] << "------------" << std::endl;
		crwl.readHtmlFromUrl(links.result[i].c_str());
		links = crwl.extractLinksFromHtml();
		file << "Total Words" << crwl.countNumberOfUniqueWordsFromHtml() << std::endl;
		if(current_depth < crwl.depth)
			crawlChilds( crwl, links, current_depth);
	}
	return 0;
}
int main() {

	Crawler crwl;
	returned_array links;
	std::cout << "Enter URL:";
	std::cin >> crwl.base_url;
	std::cout << "Enter Number of links:";
	std::cin >> crwl.link_count;
	std::cout << "Enter Depth:";
	std::cin >> crwl.depth;
	file.open("crawler.txt");
	file << "Base Level" << std::endl;
	file << "-------------------------------------------"<< std::endl;
	crwl.readHtmlFromUrl(crwl.base_url.c_str());
	links = crwl.extractLinksFromHtml();
	file << "Total Words" << crwl.countNumberOfUniqueWordsFromHtml() << std::endl;
	crawlChilds( crwl, links, 0);
	return 0;
}

