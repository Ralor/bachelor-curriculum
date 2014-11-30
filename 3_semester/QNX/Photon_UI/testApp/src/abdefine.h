/* Define header for application - AppBuilder 2.03  */

/* 'NextPermutation' Window link */
extern const int ABN_NextPermutation;
#define ABW_NextPermutation                  AbGetABW( ABN_NextPermutation )
extern const int ABN_InputString;
#define ABW_InputString                      AbGetABW( ABN_InputString )
extern const int ABN_ExitButton;
#define ABW_ExitButton                       AbGetABW( ABN_ExitButton )
extern const int ABN_RequestButton;
#define ABW_RequestButton                    AbGetABW( ABN_RequestButton )
extern const int ABN_OutputString;
#define ABW_OutputString                     AbGetABW( ABN_OutputString )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"
