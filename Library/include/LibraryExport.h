#ifndef LibraryExport_h__
#define LibraryExport_h__

#if defined(WIN32)
#ifdef  Library_EXPORTS
#define Library_EXPORT __declspec(dllexport)
#else
#define  Library_EXPORT __declspec(dllimport)
#endif

#pragma warning( disable: 4251 )

#elif __linux__
#define Library_EXPORT

#else
#define Library_EXPORT 
#endif
#endif // ExtendStructureExport_h__
