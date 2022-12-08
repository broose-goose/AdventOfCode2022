//
// Created by bruce on 12/4/2022.
//

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <regex>
#include <deque>
#include <ranges>
#include <queue>

#include "common/fs.h"

struct Tree {
    char height;
    int row;
    int col;
    bool vis_horiz = false;
    bool vis_vert = false;
    Tree(char _height, int _row, int _col): height(_height), row(_row), col(_col) {}
    bool is_vis() {
        return vis_horiz || vis_vert;
    }
};

std::ostream &operator <<(std::ostream &o, const Tree &tree)
{
    return o << "Tree(height=" << tree.height << ", [row,col]=[" << tree.row << "," << tree.col << "])";
}


std::ostream &operator <<(std::ostream &o, const std::vector<std::shared_ptr<Tree>> &height_vector)
{
    bool has_run = false;
    for (const auto &tree_p : height_vector) {
        if (tree_p == nullptr) {
            std::cout << "null";
            has_run = true;
            continue;
        }
        const auto &tree = *tree_p;
        if (has_run) {
           o << ", ";
        }
        o << tree;
        has_run = true;
    }
    return o;
}

int MaxView(
    const std::vector<std::vector<std::shared_ptr<Tree>>> &trees, const std::shared_ptr<Tree> &tree_p
) {
    int distance_left = 0, distance_right = 0, distance_top = 0, distance_bottom = 0;
    int last_row = trees.size() - 1;
    int last_col = trees.at(0).size() - 1;

    // iterate left
    auto tmp_tree_pos = tree_p->col;
    while (tmp_tree_pos > 0) {
        distance_left += 1;
        tmp_tree_pos--;
        const auto &tmp_tree_p = trees.at(tree_p->row).at(tmp_tree_pos);
        if (tmp_tree_p->height >= tree_p->height) break;
    }

    // iterate right
    tmp_tree_pos = tree_p->col;
    while (tmp_tree_pos < last_col) {
        distance_right += 1;
        tmp_tree_pos++;
        const auto &tmp_tree_p2 = trees.at(tree_p->row).at(tmp_tree_pos);
        if (tmp_tree_p2->height >= tree_p->height) break;
    }

    // top and bottom don't get pointer wizardry as we are using vectors of vectors...
    tmp_tree_pos = tree_p->row;
    while (tmp_tree_pos > 0) {
        distance_top += 1;
        tmp_tree_pos--;
        const auto &tmp_tree_p2 = trees.at(tmp_tree_pos).at( tree_p->col);
        if (tmp_tree_p2->height >= tree_p->height) break;
    }

    // iterate bottom
    tmp_tree_pos = tree_p->row;
    while (tmp_tree_pos < last_row) {
        distance_bottom += 1;
        tmp_tree_pos++;
        const auto &tmp_tree_p2 = trees.at(tmp_tree_pos).at(tree_p->col);
        if (tmp_tree_p2->height >= tree_p->height) break;
    }

    return distance_left * distance_right * distance_top * distance_bottom;
}



int ElvenElevationParsing() {
    std::cout << "Doing some elevation stuff" << std::endl;
    std::optional<Common::FileReader> op_reader = Common::FileReader::CreateFileReader("elf_elevation_map.data");
    if (!op_reader) {
        return -1;
    }
    Common::FileReader &reader = *op_reader;
    std::string scan_line;

    // read in the file
    while(reader.GetLine(scan_line)) {}
    int line_count = reader.LineCount();

    // last line is the bottom; construct min / maxes, memo; ignore edges
    std::vector<std::shared_ptr<Tree>> max_top(scan_line.size() - 2, nullptr);
    std::vector<std::shared_ptr<Tree>> max_bottom(scan_line.size() - 2, nullptr);
    std::vector<std::shared_ptr<Tree>> max_left(line_count - 2, nullptr);
    std::vector<std::shared_ptr<Tree>> max_right(line_count - 2, nullptr);
    std::vector<std::vector<std::shared_ptr<Tree>>> trees(
        line_count, std::vector<std::shared_ptr<Tree>>(scan_line.size())
    );

    // read in the edges; bottom and top we don't care about corners
    {
        const int row_position = trees.size() - 1;
        for (auto itr = scan_line.begin(); itr < scan_line.end(); itr++) {
            const int col_position = std::distance(scan_line.begin(), itr);
            auto &tree = trees.at(row_position).at(col_position);
            tree = std::make_shared<Tree>(Tree(*itr, row_position, col_position));
            if (itr != scan_line.begin() && itr != scan_line.end() - 1) {
                max_bottom.at(col_position - 1) = tree;
            }
        }
    }


    // minus 1's are to save some space, probably not worth the readability demerits tbh
    while (reader.GetLastLine(scan_line) && !reader.IsFirstLine()) {
        const int row_position = reader.LineNumber();
        for (auto itr = scan_line.begin(); itr < scan_line.end(); itr++) {
            const int col_position = std::distance(scan_line.begin(), itr);
            auto &tree = trees.at(row_position).at(col_position);
            tree = std::make_shared<Tree>(Tree(*itr, row_position, col_position));
            if (col_position == 0) {
                max_left.at(row_position - 1) = tree;
            } else if (col_position == scan_line.size() - 1) {
                max_right.at(row_position - 1) = tree;
            }
        }
    }

    // scan_line currently holds line 1, so use it
    for (auto itr = scan_line.begin() + 1; itr < scan_line.end() - 1; itr++) {
        const int col_position = std::distance(scan_line.begin(), itr);
        auto &tree = trees.at(0).at(col_position);
        tree = std::make_shared<Tree>(Tree(*itr, 0, col_position));
        if (itr != scan_line.begin() && itr != scan_line.end() - 1) {
            max_top.at(col_position - 1) = tree;
        }
    }


#ifdef DEBUG
    std::cout << "Top: " << max_top << std::endl;
    std::cout << "Left: " << max_left << std::endl;
    std::cout << "Right: " << max_right << std::endl;
    std::cout << "Bottom: " << max_bottom << std::endl;
#endif

    // okay, now we find the visible ones. O(2*(m-1)*(n-1))
    // remove corners
    int visible_total = trees.size() * 2 + trees.at(0).size() * 2 - 4;

    // vertical slicing
    {
        auto itr_top = max_top.begin();
        auto itr_bottom = max_bottom.begin();
        for(; itr_top != max_top.end() && itr_bottom != max_bottom.end(); ++itr_top, ++itr_bottom) {
            int top_position = 1;
            int bottom_position = trees.size() - 2;
            int col_position = std::distance(max_top.begin(), itr_top) + 1;
            while (top_position <= bottom_position) {
                if ((*itr_top)->height < (*itr_bottom)->height) {
                    const auto &tree_p = trees.at(top_position).at(col_position);
                    if (tree_p->height > (*itr_top)->height) {
                        // first pass so we know its not already visible
                        visible_total += 1;
                        tree_p->vis_vert = true;
                        *itr_top = tree_p;
                    }
                    top_position++;
                } else {
                    const auto &tree_p = trees.at(bottom_position ).at(col_position);
                    if (tree_p->height > (*itr_bottom)->height) {
                        visible_total += 1;
                        tree_p->vis_vert = true;
                        *itr_bottom = tree_p;
                    }
                    bottom_position--;
                }
            }
        }
    }

    // horizontal slicing
    {
        auto itr_left = max_left.begin();
        auto itr_right = max_right.begin();
        for(; itr_left != max_left.end() && itr_right != max_right.end(); ++itr_left, ++itr_right) {
            int left_position = 1;
            int right_position = trees.at(0).size() - 2;;
            int row_position = std::distance(max_left.begin(), itr_left) + 1;
            while (left_position <= right_position) {
                if ((*itr_left)->height < (*itr_right)->height) {
                    const auto &tree_p = trees.at(row_position).at(left_position);
                    if (tree_p->height > (*itr_left)->height) {
                        // second pass, pay already be visible
                        if (!tree_p->is_vis()) {
                            visible_total += 1;
                        }
                        tree_p->vis_horiz = true;
                        *itr_left = tree_p;
                    }
                    left_position++;
                } else {
                    const auto &tree_p = trees.at(row_position).at(right_position);
                    if (tree_p->height > (*itr_right)->height) {
                        if (!tree_p->is_vis()) {
                            visible_total += 1;
                        }
                        tree_p->vis_horiz = true;
                        *itr_right = tree_p;
                    }
                    right_position--;
                }
            }
        }
    }

    std::cout << "Total visible trees: " << visible_total << std::endl;

    int max_view = 0;

    // I'm convinced you could do this in O(m*n) time, but i digress...
    for (int col = 1; col < trees.at(0).size() - 1; col++) {
        for (int row = 1; row < trees.size() - 1; row++) {
            max_view = std::max(max_view, MaxView(trees, trees.at(row).at(col)));
        }
    }

    std::cout << "Max viewing distance: " << max_view << std::endl;

    return 0;
}


int main() {
    return ElvenElevationParsing();
}