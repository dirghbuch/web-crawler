# web-crawler
Web crawler in CPP to crawl links upto your desired depth. Console app

- Program compiled on Ubuntu 14.04 LTS
- Compiled with g++ compiler

Dependencies
- CURL
- Boost Libraries

Command used to compile
- g++ crawler.cpp -lcurl -lboost_regex -o crawler

Input 
- URL: URL you want to crawl example "dirghbuch.com"
- Number of links: Number of links per page you want to extract from crawling
- Depth: how deep we want to go for crawling where depth can be defined as depth of tree

Output
- crawler.txt

Limitation
- Number of links can be maximum 100
- Does not work for website which has blocked curl crawling for example google.com yahoo.com
- Lack of parallelism makes it quite slow.
- link which does not have complete URL are appended the URL inserted by user in biggining 
  e.g www.xyz.com has /conatct-us the url will be www.xyz.com/contact-us
- Unique words contain html tags as well

Improvements possible but not implemented
- Limiting use of shared variables
- Improving to make it easy to parallelize
- More efficient ways to crawl than CURL
