/*
 *  twitter.cpp
 *  Twitterc
 *
 *  Created by Christopher Joel on 4/21/10.
 *  Copyright 2010 Aether Media. All rights reserved.
 *
 */

#include "twitter.h"

using namespace aether;

Twitter *Twitter::twitterInstance = NULL;
std::string Twitter::curlResponse = "";
std::string Twitter::curlError = "Undefined";

/*
 *
 * Provides the singleton abstraction for the
 * Twitter class..
 *
 */
Twitter *Twitter::instance() {
	
	return twitterInstance = twitterInstance != NULL ? twitterInstance : new Twitter();
	
}

/*
 *
 * Utility function for reporting errors..
 *
 */
void Twitter::error(std::string message) {
	
	std::cout << "Twitter error! Message: " << message << std::endl;
	
}

/*
 *
 * Utility function for parsing responses from curl..
 *
 */
void Twitter::curlParseCode(CURLcode curlCode) {
	
	if(curlCode == CURLE_OK) {
		
		std::cout << "Twitter interaction successful! Response follows...\n\n" << twitterInstance->getResponse() << std::endl;
		
	} else {
		
		std::cout << "Curl reported an error! Code: " << curlCode << ", Error: " << Twitter::curlError << std::endl;
		
	}
	
}

/*
 *
 * Constructor; initializing members and curl..
 *
 */
Twitter::Twitter() {
	
	this->username = "";
	this->password = "";
	this->message = "";
	this->count = "5";
	
	this->response = "";
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	this->curl = curl_easy_init();
	
}

/*
 *
 * Deconstructor; de-initialize curl..
 *
 */
Twitter::~Twitter() {
	
	curl_easy_cleanup(this->curl);
	
}

/*
 *
 * Set the username for Twitter action context..
 *
 */
void Twitter::setUsername(std::string username) {
	
	this->username = username;
	
}

/*
 *
 * Set the password for status update operations..
 *
 */
void Twitter::setPassword(std::string password) {
	
	this->password = password;
	
}

/*
 *
 * Set the status for status update operations..
 *
 */
void Twitter::setMessage(std::string message) {
	
	this->message = message;
	
}

/*
 *
 * Set the number of statuses to retrieve for timeline
 * retreival operations. The default is 5..
 *
 */
void Twitter::setCount(std::string count) {
	
	this->count = count;
	
}

/*
 *
 * Parse the XML response from the Twitter API. If I had
 * the time and the motivation, the logic here would be forked off
 * into some kind of Twitter response parsing class hierarchy..
 *
 */
std::string Twitter::getResponse() {
	
	std::stringstream formattedResponse;
	
	xmlParserInputBufferPtr input = xmlAllocParserInputBuffer(XML_CHAR_ENCODING_UTF8);
	xmlBufferPtr xmlBuffer = xmlBufferCreateStatic((void *)Twitter::curlResponse.c_str(), strlen(Twitter::curlResponse.c_str()));
	
	input->buffer = xmlBuffer;
	
	xmlTextReaderPtr xml = xmlNewTextReader(input, NULL);
	
	int result = xmlTextReaderRead(xml);
	int type = xmlTextReaderNodeType(xml);
	char *name = (char *)xmlTextReaderName(xml);
	
	if(!strcmp(name, "statuses") && type != XML_READER_TYPE_END_ELEMENT) {
		
		formattedResponse << "Last " << this->count << " Twitter updates for " << this->username << ":\n\n";
		
		while((strcmp(name, "statuses")) || (!strcmp(name, "statuses") && type != XML_READER_TYPE_END_ELEMENT)) {
			
			result = xmlTextReaderRead(xml);
			type = xmlTextReaderNodeType(xml);
			name = (char *)xmlTextReaderName(xml);
			
			if(!strcmp(name, "status") && type != XML_READER_TYPE_END_ELEMENT) {
				
				formattedResponse << "=== Tweet ===\n";
				
				while((strcmp(name, "status") || (!strcmp(name, "status") && type != XML_READER_TYPE_END_ELEMENT)) && strcmp(name, "user")) {
					
					result = xmlTextReaderRead(xml);
					type = xmlTextReaderNodeType(xml);
					name = (char *)xmlTextReaderName(xml);
										
					if(strcmp(name, "#text") && type != XML_READER_TYPE_END_ELEMENT) {
						
						if(!strcmp(name, "created_at")) {
							
							formattedResponse << "Date:\n" << xmlTextReaderReadString(xml) << "\n\n";
							
						}
						
						if(!strcmp(name, "text")) {
							
							
							formattedResponse << "Status:\n" << xmlTextReaderReadString(xml) << "\n\n";
							
						}
						
					}
					
				}
				
			}
			
		}
		
	} else if(!strcmp(name, "status") && type != XML_READER_TYPE_END_ELEMENT) {
		
		formattedResponse << "The Twitter status for " << this->username << " has been updated as follows: \n\n";
		
		while((strcmp(name, "status")) || (!strcmp(name, "status") && type != XML_READER_TYPE_END_ELEMENT)) {
			
			result = xmlTextReaderRead(xml);
			type = xmlTextReaderNodeType(xml);
			name = (char *)xmlTextReaderName(xml);
			
			if(strcmp(name, "#text") && type != XML_READER_TYPE_END_ELEMENT) {
				
				if(!strcmp(name, "text")) {
					
					formattedResponse << this->username << ": \"" << xmlTextReaderReadString(xml) << "\"\n\n";
					
				}
				
			}
			
		}
		
	} else if(!strcmp(name, "hash") && type != XML_READER_TYPE_END_ELEMENT) {
		
		formattedResponse << "Twitter gave an unexpected response; dumping:\n\n";
		
		while(strcmp(name, "hash") || (!strcmp(name, "hash") && type != XML_READER_TYPE_END_ELEMENT)) {
			
			result = xmlTextReaderRead(xml);
			type = xmlTextReaderNodeType(xml);
			name = (char *)xmlTextReaderName(xml);
			
			if(strcmp(name, "#text") && type != XML_READER_TYPE_END_ELEMENT) {
				
				formattedResponse << name << ": " << xmlTextReaderReadString(xml) << "\n";
				
			}
			
		}
		
	} else {
		
		error("Unrecognized format returned from Twitter! Response will be unformatted...");
		
		formattedResponse << Twitter::curlResponse << "\n\n";
		
	}
	
	xmlFreeTextReader(xml);
	xmlFreeParserInputBuffer(input);
	
	return formattedResponse.str();
	
}

/*
 *
 * Initiate the Twitter request. Only works after at least
 * a username is specified..
 *
 */

void Twitter::tweet() {
	
	if(this->message != "") {
		
		if(this->username != "") {
			
			if(this->password != "") {
				
				this->curlSendTweet();
				
			} else {
				
				error("In order to send a tweet, you must specify the password for user " + this->username + "!");
				
			}
			
		} else {
			
			error("In order to send a tweet, you must specify your username!");
			
		}
		
	} else if(this->username != "") {
		
		this->curlGetTweets();
		
	} else {
		
		error("At bare minimum, a message tweet or a user to retrieve tweets from must be defined!");
		
	}
	
	return;
	
}

/*
 *
 * Curl operation to perform a status update request..
 *
 */
void Twitter::curlSendTweet() {
	
	std::cout << "Attempting to send tweet: \"" << this->message << "\"..." << std::endl;
	
	this->message = "status=" + this->message;
	
	curl_easy_setopt(this->curl, CURLOPT_URL, "http://api.twitter.com/1/statuses/update.xml");
	curl_easy_setopt(this->curl, CURLOPT_ERRORBUFFER, &(Twitter::curlError));
	curl_easy_setopt(this->curl, CURLOPT_USERPWD, std::string(this->username + ":" + this->password).c_str());
	curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, (this->message).c_str());
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, curlHandleResponse);
	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &(Twitter::curlResponse));
	
	curlParseCode(curl_easy_perform(this->curl));
	
}

/*
 *
 * Curl operation to perform a timeline retrieval request..
 *
 */
void Twitter::curlGetTweets() {
	
	std::cout << "Pulling timeline for " << this->username << "..." << std::endl;
	
	curl_easy_setopt(this->curl, CURLOPT_URL, std::string("http://api.twitter.com/1/statuses/user_timeline.xml?screen_name=" + this->username + "&count=" + this->count).c_str());
	curl_easy_setopt(this->curl, CURLOPT_ERRORBUFFER, &(Twitter::curlError));
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, curlHandleResponse);
	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &(Twitter::curlResponse));
	
	curlParseCode(curl_easy_perform(this->curl));
	
}

/*
 *
 * Curl response handler - simply appends the response to a member
 * string..
 *
 */
int Twitter::curlHandleResponse(char *buffer, size_t size, size_t nmemb, std::string *response) {
	
	if (response != NULL)  
	{
		
		response->append(buffer, size * nmemb);
		
	}
	
	return response != NULL ? size * nmemb : 0;
	
}