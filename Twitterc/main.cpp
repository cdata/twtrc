/*
 *  Twitterc
 *	
 *	A basic Unix-compatible command-line twitter client. Using 
 *  this client one can either update his or her status, or retrieve 
 *  the last n status updates of a given user, where n is specified
 *  by the -c option.
 *  
 *  Options:
 *  
 *    -u $USERNAME     Set username for command context
 *    -p $PASSWORD     Set authentication password
 *    -c $COUNT        Set the number of messages to return
 *    -?               Print usage
 *  
 *  This application depends on the following open-source, 
 *  cross-platform libraries: 
 *  
 *    - libcurl: http://curl.haxx.se/libcurl/
 *    - libxml2: http://xmlsoft.org/ 
 *  
 *  Created by Christopher Joel on 4/21/10.
 *  Copyright 2010 Aether Media. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "twitter.h"

using namespace aether;

void argumentError(std::string message, std::string argument) {
	
	std::cout << "Argument Error: " << argument << " - " << message << std::endl;
	
}

void printUsage() {
	
	std::cout	<< "\nTwitterc is a basic Unix-compatible command-line twitter client. Using this client one can either update his or her status, or retrieve the last n status updates of a given user, where n is specified by the -c option.\n\nUsage:\n" << std::endl
				<< "\t$ tweetc [OPTIONS [STATUS]]\n" << std::endl
				<< "\t-u $USERNAME     Set username for command context." << std::endl
				<< "\t-p $PASSWORD     Set authentication password for status update." << std::endl
				<< "\t-c $COUNT        Set the number of messages to return." << std::endl
				<< "\t-?               Print this usage information." << std::endl
				<< "\nCreated by Christopher Joel\nCopyright 2010 Aether Media. All rights reserved.\n" << std::endl;
	
}

int main (int argc, const char * argv[]) {
	
	if(argc > 1) {
	
		for(int i = 1; i < argc; i++) {
			
			if(argv[i][0] != '-') {
				
				std::stringstream message;
				
				message << argv[i++];
				
				while(i < argc && argv[i][0] != '-') {
					
					message << " " << argv[i];
					i++;
					
				}
				
				i--;
				
				Twitter::instance()->setMessage(message.str());
				
			} else {
				
				switch(argv[i][1]) {
					
					case 'u': {
						
						if(++i < argc) {
							
							Twitter::instance()->setUsername((char *)argv[i]);
							
						} else {
							
							argumentError("No username provided!", "Username");
							
						}
						
						break;
					}
					case 'p': {
						
						if(++i < argc) {
							
							Twitter::instance()->setPassword((char *)argv[i]);
							
						} else {
							
							argumentError("No password provided!", "Password");
							
						}
						
						break;
					}
					case 'c': {
						
						if(++i < argc) {
							
							Twitter::instance()->setCount((char *)argv[i]);
							
						} else {
							
							argumentError("No count specified!", "Count");
							
						}
						
						break;
					}
					case '?': {
						
						printUsage();
						
						break;
						
					}
					default: {
						
						argumentError("Unknown argument!", (char *)argv[i][1]);
						
						break;
					}
					
				}
				
			}
			
		}
		
		Twitter::instance()->tweet();
		
	} else {
		
		printUsage();
		
	}
	
	if(Twitter::instance() != NULL) {
		
		delete Twitter::instance();
		
	}
	
	return 0;
	
}

