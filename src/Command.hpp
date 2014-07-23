#ifndef __COMMAND_HPP
#define __COMMAND_HPP

#include <string>
class CommandSource;

/**
	A command that can be executed on the server.
*/
class Command{
	private:
		std::string name_;
		std::string help_;
	
	protected:
		// Constructors
		Command();
		virtual ~Command();
	
	public:
		// Properties
		/**
			The name of the command as it is called in the console.
			
			@return The command name
		*/
		virtual std::string getName()=0;
		/**
			The help description for the command.
			
			@return The help description of the command
		*/
		virtual std::string getHelp()=0;
		
		/**
			The actual functionality of the command.
			
			@param args The arguments for the command, NULL if no arguments were given
			@param source The command's source
		*/
		virtual void act(std::string* args, CommandSource* source)=0;
		/**
			A utility method for splitting a string.
			
			The second string pointer will be NULL if delimiter is not present in the string
			or if it is the last character.
			@param strToSplit The string to split
			@param first A pointer to where the first string will be stored
			@param second A pointer to where the second string will be stored
			@param del The delimiter to split with
		*/
		static void split(std::string strToSplit, std::string* first, std::string** second, char del);
};

#endif
