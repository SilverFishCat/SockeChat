#ifndef __COMMANDS_HPP
#define __COMMANDS_HPP
#include "Command.hpp"

class ExitCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~ExitCommand();
		virtual void act(std::string* args, CommandSource* source);
};

class HelpCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~HelpCommand();
		virtual void act(std::string* args, CommandSource* source);
};

class ShoutCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~ShoutCommand();
		virtual void act(std::string* args, CommandSource* source);
};

class WhisperCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~WhisperCommand();
		virtual void act(std::string* args, CommandSource* source);
};

class NickCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~NickCommand();
		virtual void act(std::string* args, CommandSource* source);
};

class ListCommand : public Command{
	private:
		static const std::string NAME;
		static const std::string HELP;
	public:
		virtual std::string getName();
		virtual std::string getHelp();
		virtual ~ListCommand();
		virtual void act(std::string* args, CommandSource* source);
};

#endif
