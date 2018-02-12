#include <iostream>
#include <vector>
#include <memory>

class Maze
{
private:
  static constexpr char N = 1;
  static constexpr char E = 2;
  static constexpr char S = 4;
  static constexpr char W = 8;

  const uint height;
  const uint width;
  
  struct Cell
  {
    const uint x, y;
    char walls = N|E;
    char freeDirs = N|S|E|W;
    Cell* parentCell = nullptr;

    Cell(uint x, uint y)
      : x(x), y(y) {}
    
    bool has_parent () { return parentCell != nullptr; }
    bool has_wall (char dir) { return walls & dir; }
  };

  std::vector<Cell> cells;
  Cell& cell_by_coordinate(uint x, uint y)
  {
    return cells[x + y * width];
  }

  void reset_cells ()
  {
    cells.clear();
    for (auto i = 0u; i < height; ++i)
      for (auto j = 0u; j < width; ++j)
      {
        auto newCell = Cell(j, i);

        if (i == 0) { newCell.freeDirs &= ~N; }
        if (i == height - 1) { newCell.freeDirs &= ~S; }
        if (j == 0) { newCell.freeDirs &= ~W; }
        if (j == width - 1) { newCell.freeDirs &= ~E; }

        cells.push_back(newCell);
      }
  }

  Cell* next_cell_dfrc (Cell* cell) //depth first reccursive backtrack
  {
    while(cell->freeDirs)
    {
      char nextDir = 1 << std::rand()%4;
      if (!(nextDir & cell->freeDirs))
        continue;

      cell->freeDirs &= ~nextDir;

      switch (nextDir)
      {
        case(N) : //NORTH
          {
            Cell* nextCell = &cell_by_coordinate(cell->x, cell->y - 1);
            if (nextCell->has_parent())
              continue;
            nextCell->parentCell = cell;
            cell->walls &= ~N;
            return nextCell;
          }
        case(E) : //EAST
          {
            Cell* nextCell = &cell_by_coordinate(cell->x + 1, cell->y);
            if (nextCell->has_parent())
              continue;
            nextCell->parentCell = cell;
            cell->walls &= ~E;
            return nextCell;
          }
        case(S) : //SOUTH
          {
            Cell* nextCell = &cell_by_coordinate(cell->x, cell->y + 1);
            if (nextCell->has_parent())
              continue;
            nextCell->parentCell = cell;
            nextCell->walls &= ~N;
            return nextCell;
          }
         case(W) : //WEST
          {
            Cell* nextCell = &cell_by_coordinate(cell->x - 1, cell->y);
            if (nextCell->has_parent())
              continue;
            nextCell->parentCell = cell;
            nextCell->walls &= ~E;
            return nextCell;
          }
      }
    }
    return cell->parentCell;
  }

  void depth_first_reccursive_backtrack()
  {
    auto cellPtr = &cell_by_coordinate(0, 0);
    cellPtr->parentCell = cellPtr;
    do
    {
      cellPtr = next_cell_dfrc(cellPtr);
    }
    while(cellPtr != cellPtr->parentCell);
  }
  
public:
  void generate_maze()
  {
    reset_cells();

    depth_first_reccursive_backtrack();
  }

  void print_to_cout(char c)
  {
    for (auto i = 0u; i < height; ++i)
    {
      std::string tmp;
      tmp.append(1 + width * 2, c);
      tmp += '\n';
      tmp.append(1 + width * 2, c);

      for (auto j = 0u; j < width; ++j)
      {
        tmp[3 + (j + width) * 2 ] = ' ';

        if (!(cell_by_coordinate(j, i).has_wall(N)))
          { tmp[1 + j * 2] = ' '; }
        if (!(cell_by_coordinate(j, i).has_wall(E))) 
          { tmp[4 + (j + width) * 2] = ' '; }
      }
      std::cout << tmp << std::endl;
    }
    std::cout << std::string(1 + width * 2, c) << std::endl;
  }

  Maze(uint height, uint width)
    : height(height), width(width)
  {
        generate_maze();
  }
};


int main(int, char** argv)
{
  auto maMaze = Maze(atoi(argv[1]), atoi(argv[2]));
  maMaze.print_to_cout('#');
}
