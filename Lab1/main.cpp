#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using sequence = std::string;

struct SeqAlignment {
  SeqAlignment(const sequence& s0, const sequence& s1,
               int score_match, int score_mismatch, int score_indel);

  enum MatchType { Match, MisMatch, Gap0, Gap1 };
  std::vector<MatchType> alignment;
  const sequence& s0;
  const sequence& s1;
  int score_match;
  int score_mismatch;
  int score_indel;
};

SeqAlignment::SeqAlignment(const sequence& s0, const sequence& s1,
                           int score_match, int score_mismatch, int score_indel)
        : s0(s0), s1(s1),
          score_match(score_match), score_mismatch(score_mismatch), score_indel(score_indel) {
    // TODO: Compute alignment and store it to `this->alignment`.
}

std::ostream& operator<< (std::ofstream& ofs, SeqAlignment as) {
    // TODO: Print result to `ofs`.
    return ofs;
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [input-file] [output-file]" << std::endl;
        exit(1);
    }

    int score_match, score_mismatch, score_indel;
    sequence sequence0, sequence1;
    std::ifstream ifs(argv[1], std::ifstream::in);
    std::ofstream ofs(argv[2], std::ofstream::out);

    ifs >> score_match >> score_mismatch >> score_indel;
    ifs >> sequence0 >> sequence1;

    SeqAlignment alignment_0_1(sequence0, sequence1, score_match, score_mismatch, score_indel);

    ofs << alignment_0_1;

    return 0;
}