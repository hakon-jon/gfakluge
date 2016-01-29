
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
namespace gfak{
    struct header_elem{
        std::string key;
        std::string val;
    };

    struct annotation_elem{
        std::string key;
        std::string info;
    };

		struct alignment_elem{
				std::string source_name;
				int position;
				std::string ref;
				bool source_orientation_forward;
				int length;
		};

    struct sequence_elem{
        // A list of links to neighbors

        // Node sequence
        std::string seq;
        std::string name;
        long id;

    };

    struct link_elem{

        // Source sequence
        std::string source_name;
        // Sink sequence
        std::string sink_name;
        bool source_orientation_forward;
        bool sink_orientation_forward;
        std::string cigar;
    };

    struct contained_elem{
        std::string source_name;
        std::string sink_name;
        bool source_orientation_forward;
        bool sink_orientation_forward;
        int pos;
        std::string cigar;
    };

    class GFAKluge{
			friend std::ostream& operator<<(std::ostream& os, GFAKluge& g);

        public:
            GFAKluge();
            ~GFAKluge();
            bool parse_gfa_file(std::string filename);
            bool parse_gfa_file(std::fstream gfa_stream);

            void add_link(string seq_name, link_elem);
            void add_link(sequence_elem s, link_elem l);
            void add_contained(string seq_name, contained_elem c);
            void add_contained(sequence_elem s, contained_elem c);
            void add_alignment(string s, alignment_elem a);
            void add_alignment(sequence_elem s, alignment_elem a);

            vector<link_elem> get_links(sequence_elem seq);
            vector<link_elem> get_links(string seq_name);

            vector<contained_elem> get_contained(string seq_name);
            vector<contained_elem> get_contained(sequence_elem seq);

            vector<alignment_elem> get_alignments(string seq_name);
            vector<alignment_elem> get_alignments(sequence_elem seq);

            map<string, sequence_elem> get_name_to_seq();
            map<string, string> get_header();


            std::string to_string();

        private:
            map<std::string, std::string> header;
            map<std::string, vector<contained_elem> > seq_to_contained;
            map<std::string, vector<link_elem> > seq_to_link;
            //Since we can't compare sequence elements for hashing,
            // we cheat and use their names (which are only sort of guaranteed to be
            // unique.
            map<string, sequence_elem> name_to_seq;
            std::vector<std::string> split(string s, char delim);
            map<string, vector<alignment_elem> > seq_to_alignment;


    };

};
