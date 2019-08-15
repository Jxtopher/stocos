///
/// @file evalJobShopProblem.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief https://developers.google.com/optimization/scheduling/job_shop
///

#ifndef EVALJOBSHOPPROBLEM_H
#define EVALJOBSHOPPROBLEM_H

#include <jsoncpp/json/json.h>
#include <fstream>
#include <map>
#include <utility>

#include "../solution/solutionArray.h"
#include "problem.h"

using TYPE_FITNESS_JOBSHOPPROBLEM = double;
using TYPE_CELL_JOBSHOPPROBLEM = bool;
using SOL_JOBSHOPPROBLEM = SolutionArray<TYPE_FITNESS_JOBSHOPPROBLEM, TYPE_CELL_JOBSHOPPROBLEM>;
class JobShopProblem : public Problem<SOL_JOBSHOPPROBLEM, TYPE_FITNESS_JOBSHOPPROBLEM, TYPE_CELL_JOBSHOPPROBLEM> {
   public:
    JobShopProblem(string pathfile_instance) { loadInstance(pathfile_instance); }

    ~JobShopProblem() {}

    void loadInstance(const string &file) {
        Json::Value root;  // will contains the root value after parsing.
        Json::Reader reader;
        std::ifstream test(file, std::ifstream::binary);
        bool parsingSuccessful = reader.parse(test, root, false);

        if (!parsingSuccessful) throw runtime_error(reader.getFormattedErrorMessages());

        std::string encoding = root.get("encoding", "UTF-8").asString();
        // cout<<root<<endl;
        // for (auto name : root["problem"].getMemberNames()) {
        //     cout<<name<<endl;
        // }

        // _N = root["problem"]["N"].asUInt();
    }

    friend std::ostream &operator<<(std::ostream &out, const JobShopProblem &e) {
        cout << "job X : (machine_id, processing_time)";
        vector<vector<pair<unsigned int, unsigned int>>> const _instance = e.getInstance();
        for (unsigned int i = 0; i < _instance.size(); i++) {
            out << endl << "job " << i << ":" << endl;
            for (unsigned int j = 0; j < _instance[i].size(); j++) {
                out << "(" << _instance[i][j].first << ", " << _instance[i][j].second << "), ";
            }
        }
        return out;
    }

    const vector<vector<pair<unsigned int, unsigned int>>> &getInstance() const { return instance; }

    void full_eval(SOL_JOBSHOPPROBLEM &s) const {}

    /*void incremental(SolutionArray const &s, unsigned int mutatedCell) const {

    }*/

    void reset_solution(SOL_JOBSHOPPROBLEM &s) const {}

   private:
    vector<vector<pair<unsigned int, unsigned int>>> instance;
};

#endif