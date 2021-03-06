#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cctype>
#include <locale>

#include "tokenizer.hpp"

#ifndef __INFOPARSER_HPP__
#define __INFOPARSER_HPP__
/*
    Trimming code from:
    http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
*/
// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

class InfoFileParser
{
    private:
        std::string infoFile;
        std::string basename;
        bool strict;
        std::ifstream infoStream;

        std::unordered_map<std::string, std::string> dict; // key-value pairs

        /*
            Special values that we will need later.
        */
        size_t totalFrames;

        void extractFromKVPairs()
        {
            /*
                Set some members of this class according to what we stored in
                the dictionary.
            */
//            std:string k;

//            k = "totalFrames";
//            if (std::find(dict.begin(), dict.end(), k) != dict.end()) {
//                totalFrames = std::stoull(dict[k]);
//            }

            totalFrames = dict.size();

            return;
        }

        std::string getBasename(std::string const &path)
        {
            std::string r;
            size_t idx = path.find_last_of('/');
            if (idx != path.npos) {
                r = path.substr(0, idx);
            } else {
                r = "";
            }
            return r;
        }

    public:
        InfoFileParser(bool _strict = true)
            : strict(_strict), totalFrames(0)
        {
            return;
        }

        InfoFileParser(std::string const &_infoFile, bool _strict = true)
            : infoFile(_infoFile), strict(_strict), totalFrames(0)
        {
            readInfoFile(infoFile);
            return;
        }

        ~InfoFileParser()
        {
            return;
        }

        void readInfoFile(std::string const &cfgfile)
        {
            infoStream.open(cfgfile);
            size_t line = 0;
            do {
                line++;
                std::vector<std::string> tokens = Tokenizer::splitNextLine(infoStream, '=');
                if (infoStream.eof() || !infoStream.good()) {
                    break;
                }
                if (tokens.size() == 0 || tokens[0][0] == '#') {
                    // comment or blank line; ignore it
                    continue;
                }
                if (tokens.size() == 2) {
                    dict[trim(tokens[0])] = ltrim(tokens[1]);
                } else {
                    if (strict) {
                        std::cerr << "FATAL: line " << line;
                        std::cerr << " has incorrect number of tokens (";
                        std::cerr << tokens.size() << ") [";
                        for (auto const &t : tokens) {
                            std::cerr << " " << t;
                        }
                        std::cerr<< " ]" << std::endl;
                        exit(1);
                    } else {
                        std::cerr << "Ignoring line with " << tokens.size() << " tokens." << std::endl;
                    }
                }
            } while (true);
            infoStream.close();
            extractFromKVPairs();
            basename = getBasename(cfgfile);
            return;
        }

        size_t getTotalFrames() { return totalFrames; }

        std::string getFrameCSVFilename(size_t frame) { return basename+"/fp_"+std::to_string(frame)+".h.csv"; }

        friend std::ostream& operator<<(std::ostream& os, const InfoFileParser& ip)
        {
            os << "[\n";
            for (auto const & kv : ip.dict) {
                os << "    [" << kv.first << " = " << kv.second << "]\n";
            }
            os << "]";
            return os;
        }
};

#endif //__INFOPARSER_HPP__

