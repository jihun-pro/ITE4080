#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using sequence = std::string;

/*** Start of `struct SeqAlignment`. ***/

struct SeqAlignment {
  SeqAlignment(const sequence& s0, const sequence& s1,
               int score_match, int score_mismatch, int score_indel);

  enum MatchType {
    Match, MisMatch, Gap0, Gap1
  };
  std::vector<MatchType> alignment;
  int num_match;
  __unused int max_score;

  const sequence& s0;
  const sequence& s1;
  __unused int score_match;
  __unused int score_mismatch;
  __unused int score_indel;
};

SeqAlignment::SeqAlignment(const sequence& s0, const sequence& s1,
                           int score_match, int score_mismatch, int score_indel)
        : s0(s0), s1(s1),
          score_match(score_match), score_mismatch(score_mismatch), score_indel(score_indel) {
    int score[s0.size() + 1][s1.size() + 1];
    std::pair<int, int> prev[s0.size() + 1][s1.size() + 1];

    // Initialization
    for (int i = 0; i <= s0.size(); i++) {
        score[i][0] = 0;
        prev[i][0] = {i - 1, 0};
    }
    for (int i = 0; i <= s1.size(); i++) {
        score[0][i] = 0;
        prev[0][i] = {0, i - 1};
    }

    // Compute optimal alignment
    for (int i = 1; i <= s0.size(); i++) {
        for (int j = 1; j <= s1.size(); j++) {
            score[i][j] = score[i - 1][j - 1] + (s0[i - 1] == s1[j - 1] ? score_match : score_mismatch);
            prev[i][j] = {i - 1, j - 1};
            if (score[i][j] < score[i - 1][j] + score_indel) {
                score[i][j] = score[i - 1][j] + score_indel;
                prev[i][j] = {i - 1, j};
            }
            if (score[i][j] < score[i][j - 1] + score_indel) {
                score[i][j] = score[i][j - 1] + score_indel;
                prev[i][j] = {i, j - 1};
            }
        }
    }
    max_score = score[s0.size()][s1.size()];

    // Backtrack
    std::pair<int, int> pos = {s0.size(), s1.size()};
    while (pos.first || pos.second) {
        auto prev_pos = prev[pos.first][pos.second];
        if (prev_pos.first == pos.first)
            alignment.push_back(MatchType::Gap1);
        else if (prev_pos.second == pos.second)
            alignment.push_back(MatchType::Gap0);
        else if (s0[pos.first - 1] == s1[pos.second - 1])
            alignment.push_back(MatchType::Match), num_match++;
        else
            alignment.push_back(MatchType::MisMatch);
        pos = prev_pos;
    }
    std::reverse(alignment.begin(), alignment.end());
}

std::ostream& operator<<(std::ostream& ofs, const SeqAlignment& as) {
    ofs << as.num_match << std::endl;

    // TODO: Print result to `ofs`.
    return ofs;
}

/*** End of `struct SeqAlignment`. ***/

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " [input-file] [output-file]" << std::endl;
        exit(1);
    }

    int score_match, score_mismatch, score_indel;
    sequence sequence0, sequence1;
    std::ifstream ifs(argv[1], std::ifstream::in);
    std::ofstream ofs(argv[2], std::ofstream::out);
    if (!ifs.good() || !ifs.good()) {
        std::cerr << "Error while opening file" << std::endl;
        exit(1);
    }

    ifs >> score_match >> score_mismatch >> score_indel;
    ifs >> sequence0 >> sequence1;
    ifs.close();

    SeqAlignment alignment_0_1(sequence0, sequence1, score_match, score_mismatch, score_indel);

    ofs << alignment_0_1;
    ofs.close();

    return 0;
}