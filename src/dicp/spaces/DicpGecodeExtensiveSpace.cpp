//
// Created by Ryan J. O'Neil on 5/8/16.
//

#include <map>
#include <vector>
#include "DicpGecodeExtensiveSpace.h"

using namespace Gecode;
using namespace std;

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(DicpProblem problem) : problem{problem} {
    size_t num_images = problem.get_images().size();
    size_t num_commands = problem.get_commands().size();
    size_t num_stages = problem.num_stages();

    zero = IntVar{*this, 0, 0};
    schedules = map<dicp_image_key, IntVarArray>{ };
    domains = map<dicp_image_key, IntSet>{ };
    source = BoolVarArray{*this, (int) num_commands, 0, 1};
    for (int i = 0; i < (num_stages - 1); ++i) {
        vector<BoolVarArray> arcs2{ };
        for (int j = 0; j < num_commands; ++j)
            arcs2.push_back(BoolVarArray{*this, (int) num_commands, 0, 1});
        arcs.push_back(arcs2);
    }
    for (int i = 0; i < num_stages; ++i)
        nodes.push_back(IntVarArray{*this, (int) num_commands, 0, (int) num_images});
    for (int i = 0; i < (num_stages - 1); ++i) {
        vector<IntVarArray> entries2{ };
        for (int j = 0; j < num_commands; ++j)
            entries2.push_back(IntVarArray{*this, (int) num_commands, 0, (int) num_images});
        entries.push_back(entries2);
    }
    stage_costs = IntVarArray{*this, (int) num_stages, 0, Int::Limits::max};
    total_cost = IntVar{*this, 0, Int::Limits::max};

    // Vectors relating schedule variables to source arcs in the network
    map<int, BoolExpr> source_map{ };

    // Vectors relating schedule variables to arc usage in the network
    vector<vector<map<int, BoolExpr>>> arc_vecs{ };
    for (int i = 0; i < (num_stages - 1); ++i) {
        vector<map<int, BoolExpr>> arcs2{ };
        for (int j = 0; j < num_commands; ++j) {
            map<int, BoolExpr> arcs3{ };
            arcs2.push_back(arcs3);
        }
        arc_vecs.push_back(arcs2);
    }

    // Schedule for each individual image
    vector<DicpImage> images = problem.get_images();
    for (vector<DicpImage>::iterator it = images.begin(); it != images.end(); ++it) {

        // Domains for image schedule slots
        vector<int> command_ids{ };
        vector<DicpCommand> commands = it->get_commands();

        for (vector<DicpCommand>::iterator cit = commands.begin(); cit != commands.end(); ++cit)
            command_ids.push_back(cit->id);

        IntSet domain{&command_ids[0], (int) command_ids.size()};
        pair<dicp_image_key, IntSet> pd{it->image, domain};
        domains.insert(pd);

        // Actual schedule
        IntVarArray a{*this, it->size(), domain};
        pair<dicp_image_key, IntVarArray> p{it->image, a};
        schedules.insert(p);

        // Each command once
        distinct(*this, p.second);

        // First command requires an arc from the source
        for (vector<DicpCommand>::iterator cit = commands.begin(); cit != commands.end(); ++cit) {
            int cid = cit->id;
            if (source_map.find(cid) == source_map.end())
                source_map[cid] = expr(*this, a[0] == cid);
            else
            source_map[cid] = expr(*this, source_map[cid] || (a[0] == cid));
        }

        // Arcs
        for (int i = 0; i < (num_stages - 1); ++i)
            for (int j = 0; j < num_commands; ++j)
                for (int k = 0; k < num_commands; ++k)
                    if (j != k && (a.size() > i + 1)) {
                        if (arc_vecs[i][j].find(k) == arc_vecs[i][j].end())
                            arc_vecs[i][j][k] = expr(*this, (a[i] == j && a[i + 1] == k));
                        else
                            arc_vecs[i][j][k] = expr(*this, arc_vecs[i][j][k] || (a[i] == j && a[i + 1] == k));
                    }

        // Branching for image schedules
        branch(*this, p.second, INT_VAR_ACTIVITY_SIZE_MAX(), INT_VAL_SPLIT_MIN());
    }

    // Source
    for (int i = 0; i < num_commands; ++i)
        if (source_map.find(i) == source_map.end())
            rel(*this, !source[i]);
        else
            rel(*this, expr(*this, source_map[i]) == source[i]);

    // Arcs
    for (int i = 0; i < (num_stages - 1); ++i)
        for (int j = 0; j < num_commands; ++j)
            for (int k = 0; k < num_commands; ++k)
                if (j == k || arc_vecs[i][j].find(k) == arc_vecs[i][j].end())
                    rel(*this, !arcs[i][j][k]);
                else
                    rel(*this, expr(*this, arc_vecs[i][j][k]) == arcs[i][j][k]);

    // Nodes
    for (int i = 0; i < num_commands; ++i)
        rel(*this, nodes[0][i] == source[i]);

    for (int i = 0; i < (num_stages - 1); ++i)
        for (int j = 0; j < num_commands; ++j) {
            vector<int> e{ };
            for (int k = 0; k < num_commands; ++k)
                if (j == k)
                    e.push_back(0);
                else
                    e.push_back(1);

            vector<IntVar> x{ };
            for (int k = 0; k < num_commands; ++k)
                x.push_back(entries[i][k][j]);
            linear(*this, e, x, IRT_EQ, nodes[i+1][j]);
        }

    // Entries
    for (int i = 0; i < (num_stages - 1); ++i)
        for (int j = 0; j < num_commands; ++j)
            for (int k = 0; k < num_commands; ++k)
                if (j == k)
                    rel(*this, entries[i][j][k] == 0);
                else
                    ite(*this, arcs[i][j][k], nodes[i][j], zero, entries[i][j][k]);

    // Stage costs
    vector<DicpCommand> cmds = problem.get_commands();
    vector<int> costs{ };
    for (vector<DicpCommand>::iterator it = cmds.begin(); it != cmds.end(); ++it)
        costs.push_back(it->cost);

    for (int i = 0; i < num_stages; ++i)
        linear(*this, costs, nodes[i], IRT_EQ, stage_costs[i]);

    // Total cost
    vector<int> es{ };
    for (int i = 0; i < num_stages; ++i)
        es.push_back(1);

    linear(*this, es, stage_costs, IRT_EQ, total_cost);
}

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s) :
        IntMinimizeSpace(share, s),
        problem{s.problem},
        zero{s.zero},
        schedules{s.schedules},
        domains{s.domains},
        source{s.source},
        arcs{s.arcs},
        nodes{s.nodes},
        entries{s.entries},
        stage_costs{s.stage_costs},
        total_cost{s.total_cost} {

    for (map<dicp_image_key, IntVarArray>::iterator it = s.schedules.begin(); it != s.schedules.end(); ++it)
        schedules.find(it->first)->second.update(*this, share, it->second);
    source.update(*this, share, s.source);
    for (int i = 0; i < s.arcs.size(); ++i)
        for (int j = 0; j < s.arcs[i].size(); ++j)
            arcs[i][j].update(*this, share, s.arcs[i][j]);
    for (int i = 0; i < s.nodes.size(); ++i)
        nodes[i].update(*this, share, s.nodes[i]);
    for (int i = 0; i < s.entries.size(); ++i)
        for (int j = 0; j < s.entries[i].size(); ++j)
            entries[i][j].update(*this, share, s.entries[i][j]);
    stage_costs.update(*this, share, s.stage_costs);
    total_cost.update(*this, share, s.total_cost);
}

Space* DicpGecodeExtensiveSpace::copy(bool share) {
    return new DicpGecodeExtensiveSpace(share, *this);
}

IntVar DicpGecodeExtensiveSpace::cost(void) const {
    return total_cost;
}

void DicpGecodeExtensiveSpace::constrain(const Space& _b) {
    const DicpGecodeExtensiveSpace& b = static_cast<const DicpGecodeExtensiveSpace&>(_b);
    rel(*this, total_cost < b.total_cost.val());
}

void DicpGecodeExtensiveSpace::print(void) const {
    DicpProblem p{problem};
    for (map<dicp_image_key, IntVarArray>::const_iterator it = schedules.begin(); it != schedules.end(); ++it) {
        cout << "[image " << it->first << "] ";
        for (IntVarArray::const_iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
            cout << p.get_command(sit->val()).command << " ";
        cout << endl;
    }
    cout << "[source] " << source << endl;
    for (int i = 0; i < arcs.size(); ++i)
        for (int j = 0; j < arcs[i].size(); ++j)
            cout << "[arcs(" << i << "," << j << ")] " << arcs[i][j] << endl;
    for (int i = 0; i < nodes.size(); ++i)
        cout << "[nodes " << i << "] " << nodes[i] << endl;
    for (int i = 0; i < entries.size(); ++i)
        for (int j = 0; j < entries[i].size(); ++j)
            cout << "[entries(" << i << "," << j << ")] " << entries[i][j] << endl;
    cout << "[stage costs] " << stage_costs << endl;
    cout << "[total cost ] " << total_cost << endl;
    cout << "---------------" << endl;
}