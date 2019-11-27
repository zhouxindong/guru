// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GURU_MEMORY_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GURU_MEMORY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GURU_MEMORY_EXPORTS
#define GURU_MEMORY_API __declspec(dllexport)
#else
#define GURU_MEMORY_API __declspec(dllimport)
#endif

// This class is exported from the guru_memory.dll
class GURU_MEMORY_API Cguru_memory {
public:
	Cguru_memory(void);
	// TODO: add your methods here.
};

extern GURU_MEMORY_API int nguru_memory;

GURU_MEMORY_API int fnguru_memory(void);
