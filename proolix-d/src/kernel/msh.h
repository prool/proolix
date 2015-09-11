#define MenuColumn 68

struct DeskTop
  {
  int x;
  int y;
  char Name [13];
  };

struct DeskTop Folder [MAX_LINE];

struct DeskTop Desk [ ] = {
{MenuColumn, 4," [Click] "},
{MenuColumn, 5," [Exec] "},
{MenuColumn, 6," [Attr] "},
{MenuColumn, 7," [Cls] "},
{MenuColumn, 8," [Mem] "},
{MenuColumn, 9," [Ls -l] "},
{MenuColumn,10," [Help] "},
{MenuColumn,11," [Echo] "},
{MenuColumn,12," [Vectors] "},
{MenuColumn,13," [Ladder] "},
{MenuColumn,14," [Reboot] "},
{MenuColumn,15," [Int 0] "},
{MenuColumn,16," [Int 18h] "},
{MenuColumn,17," [Int 19h] "},
{MenuColumn,18," [Com Addr] "},
{-1,-1,""}
};

int DeskNo;
