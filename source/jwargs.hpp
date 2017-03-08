#ifndef JWARGS_HPP
#define JWARGS_HPP

#include <string>
#include <map>
#include "args.hxx"

namespace jwargs {
   std::map<std::string, std::string> argvToMap(int argc, char *argv[]);

   class ChoiceFlag : public args::ValueFlag<std::string> {
     private:
       const std::set<std::string> options;
       std::string optionString;
     public:
         ChoiceFlag(args::Group &group_, const std::string &name_,
             const std::string &help_, args::Matcher &&matcher_,
             const std::set<std::string> &options_ = std::set<std::string>(),
             const std::string &defaultValue_ = std::string(),
             const bool extraError_ = false):
           args::ValueFlag<std::string>(group_, name_, help_,
              std::move(matcher_), defaultValue_, extraError_), options(options_) {
                std::ostringstream os;
                for (std::set<std::string>::iterator i = options.begin(); i != options.end(); ++i) {
                  if (i != options.begin()) os << " ";
                  os << *i;
                }
                optionString = os.str();
         }

         void ParseValue(const std::string &value_) override {
#ifdef ARGS_NOEXCEPT
             if (!std::reader(name, value_, this->value)) {
                 error = args::Error::Parse;
             } else if (options.find(value) == options.end()) {
                 error = args::Error::Parse;
             }
#else
             reader(name, value_, this->value);
             if (options.find(value) == options.end()) {
                 throw args::ValidationError(std::string("Invalid option: ") +
                  value + "\n(Valid options are: " + optionString + ")");
             }
#endif
         }
   };

}

#endif
