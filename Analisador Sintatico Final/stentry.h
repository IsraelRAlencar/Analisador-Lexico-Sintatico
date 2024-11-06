#include "superheader.h"  // Instead of individual includes


class STEntry 
{
    public:
        Token* token;
        bool reserved;
    
        STEntry();
        STEntry(Token*);   
        STEntry(Token*, bool);
};
