/*
 *  twitter.h
 *  tweetc
 *
 *  Created by Christopher Joel on 4/21/10.
 *  Copyright 2010 Aether Media. All rights reserved.
 *
 */

#ifndef TWITTER_H
#define TWITTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <libxml/xmlreader.h>

namespace aether {
	
	class Twitter {
		
	protected:
		
		static std::string curlResponse;
		static std::string curlError;
		
		static void error(std::string message);
		static void curlParseCode(CURLcode curlError);
		static int curlHandleResponse(char *buffer, size_t size, size_t nmemb, std::string *response);
		
		void curlSendTweet();
		void curlGetTweets();
		
		std::string username;
		std::string password;
		std::string message;
		std::string count;
		
		std::string response;
		
		CURL *curl;
		
	private:
		
		static Twitter *twitterInstance;
		
		Twitter();
		
	public:
		
		static Twitter *instance();
		
		void setUsername(std::string username);
		void setPassword(std::string password);
		void setMessage(std::string message);
		void setCount(std::string count);
		std::string getResponse();
		
		void tweet();
		
		~Twitter();
		
	};
	
}

#endif