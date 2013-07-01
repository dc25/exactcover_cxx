#include "pentominos.h"
#include "utilities/ARRAYSIZE.h"

using namespace std;

BoolPicSet L_pics =
{
	{ 
		{ 1,1,1,1 },
		{ 1 },
	},
	{ 
		{ 1 },
		{ 1 },
		{ 1 },
		{ 1,1 },
	},
};

BoolPicSet Y_pics =
{
	{ 
		{ 1,1,1,1 },
		{ 0,1 },
	},

	{ 
		{ 1,1,1,1 },
		{ 0,0,1},
	},

	{ 
		{ 0,1 },
		{ 1,1,1,1 },
	},

	{ 
		{ 0,0,1},
		{ 1,1,1,1 },
	},

	{ 
		{ 1 },
		{ 1 },
		{ 1,1 },
		{ 1 },
	},

	{ 
		{ 1 },
		{ 1,1 },
		{ 1 },
		{ 1 },
	},

	{ 
		{ 0,1 },
		{ 0,1 },
		{ 1,1 },
		{ 0,1 },
	},

	{ 
		{ 0,1 },
		{ 1,1 },
		{ 0,1 },
		{ 0,1 },
	},
};

/* ----------------------------------- */
BoolPicSet P_pics =
{
    { 
        { 1,1,1 },
        { 1,1 },
    },

    { 
        { 1,1,1 },
        { 0,1,1},
    },

    { 
        { 1,1 },
        { 1,1,1 },
    },

    { 
        { 0,1,1},
        { 1,1,1 },
    },

    { 
        { 1 },
        { 1,1 },
        { 1,1 },
    },

    { 
        { 1,1 },
        { 1,1 },
        { 1 },
    },

    { 
        { 0,1 },
        { 1,1 },
        { 1,1 },
    },

    { 
        { 1,1 },
        { 1,1 },
        { 0,1 },
    },
};

/* ----------------------------------- */
BoolPicSet U_pics =
{
    { 
        { 1,1,1 },
        { 1,0,1 } },

    { 
        { 1,0,1 },
        { 1,1,1 },
    },

    { 
        { 1,1 },
        { 0,1 },
        { 1,1 },
    },

    { 
        { 1,1 },
        { 1,0 },
        { 1,1 },
    },
};

/* ----------------------------------- */
BoolPicSet S_pics =
{
    { 
        { 1,1,1 },
        { 0,0,1,1 },
    },

    { 
        { 0,0,1,1 },
        { 1,1,1 },
    },

    { 
        { 0,1,1,1 },
        { 1,1 },
    },

    { 
        { 1,1 },
        { 0,1,1,1 },
    },

    { 
        { 1,0 },
        { 1,0 },
        { 1,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 1,1 },
        { 1,0 },
        { 1,0 },
    },

    { 
        { 0,1 },
        { 0,1 },
        { 1,1 },
        { 1,0 },
    },


    { 
        { 1,0 },
        { 1,1 },
        { 0,1 },
        { 0,1 },
    },
};



/* ----------------------------------- */
BoolPicSet V_pics =
{
    { 
        { 1,1,1 },
        { 1 },
        { 1 },
    },

    { 
        { 1 },
        { 1 },
        { 1,1,1 },
    },

    { 
        { 1,1,1 },
        { 0,0,1 },
        { 0,0,1 },
    },

    { 
        { 0,0,1 },
        { 0,0,1 },
        { 1,1,1 },
    },
};


/* ----------------------------------- */
BoolPicSet T_pics =
{
    { 
        { 1,1,1 },
        { 0,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 0,1 },
        { 1,1,1 },
    },

    { 
        { 1 },
        { 1,1,1 },
        { 1 },
    },

    { 
        { 0,0,1 },
        { 1,1,1 },
        { 0,0,1 },
    },
};
 
/* ----------------------------------- */
BoolPicSet F_pics =
{
    { 
        { 1,1 },
        { 0,1,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 0,1,1 },
        { 1,1 },
    },

    { 
        { 0,1,1 },
        { 1,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 1,1 },
        { 0,1,1 },
    },

    { 
        { 1,0 },
        { 1,1,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 1,1,1 },
        { 1,0 },
    },

    { 
        { 0,0,1 },
        { 1,1,1 },
        { 0,1 },
    },

    { 
        { 0,1 },
        { 1,1,1 },
        { 0,0,1 },
    },
};


/* ----------------------------------- */
BoolPicSet W_pics =
{
    { 
        { 1,1 },
        { 0,1,1 },
        { 0,0,1 },
    },

    { 
        { 0,0,1 },
        { 0,1,1 },
        { 1,1 },
    },

    { 
        { 1 },
        { 1,1 },
        { 0,1,1 },
    },

    { 
        { 0,1,1 },
        { 1,1 },
        { 1 },
    },
};

/* ----------------------------------- */
BoolPicSet Z_pics =
{
    { 
        { 1,1 },
        { 0,1 },
        { 0,1,1 },
    },

    { 
        { 0,1,1 },
        { 0,1 },
        { 1,1 },
    },

    { 
        { 1 },
        { 1,1,1 },
        { 0,0,1 },
    },

    { 
        { 0,0,1 },
        { 1,1,1 },
        { 1 },
    },
};

/* ----------------------------------- */
BoolPicSet X_pics =
{
    { 
        { 0,1 },
        { 1,1,1 },
        { 0,1 },
    },
};

/* ----------------------------------- */
BoolPicSet I_pics =
{
    { { 1,1,1,1,1 } },

    { { 1 },
        { 1 },
        { 1 },
        { 1 },
        { 1 } },
};

const Pentomino pentominos[] = { 
    Pentomino("L", L_pics ),
    Pentomino("Y", Y_pics ),
    Pentomino("P", P_pics ),
    Pentomino("U", U_pics ),
    Pentomino("S", S_pics ),
    Pentomino("V", V_pics ),
    Pentomino("T", T_pics ),
    Pentomino("F", F_pics ),
    Pentomino("W", W_pics ),
    Pentomino("Z", Z_pics ),
    Pentomino("X", X_pics ),
    Pentomino("I", I_pics )
};


