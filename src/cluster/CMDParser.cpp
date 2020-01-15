//
//  CMDParser.cpp
//  Project-Cluster
//
//  Created by <author> on 18/11/2019.
//
//

#include "CMDParser.hpp"

CMDParser::CMDParser()  {}

void CMDParser::getArgs(int argc,char **argv,std::string &input_file,std::string &config_file,std::string &output_file,bool &complete ) {
    /* Parse cmd line parameters using getopts
     * Modified example found at :
     * https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example
     */
    complete = false ;
    int c ;
    while (1) {
        static struct option long_options[] =
        {
            /* These options don’t set a flag.
            We distinguish them by their indices. */
            {"i"         , required_argument , 0 , 'i'},
            {"c"        , required_argument , 0 , 'c'},
            {"o"    , required_argument , 0 , 'o'},
            {"complete"    , optional_argument , 0 , 'p'},
            {0, 0, 0, 0}
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "i:c:o:p::",
        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;
        switch (c) {
            case 'i':
                input_file.insert(0,optarg);
                break;
            case 'c':
                config_file.insert(0,optarg);
                break;
            case 'o':
                output_file.insert(0,optarg);
                break;
            case 'p' :
                complete = true ;
                break ;
            case '?':
            /* getopt_long already printed an error message. */
                break;
            default:
                abort ();
        }
    }
}


CMDParser::~CMDParser() {}
