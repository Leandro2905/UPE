1 stdcall AVIBuildFilter(str long long) AVIBuildFilterA
2 stdcall AVIBuildFilterA(str long long)
3 stdcall AVIBuildFilterW(wstr long long)
4 stdcall AVIClearClipboard()
5 stdcall AVIFileAddRef(ptr)
6 stdcall AVIFileCreateStream(ptr ptr ptr) AVIFileCreateStreamW
7 stdcall AVIFileCreateStreamA(ptr ptr ptr)
8 stdcall AVIFileCreateStreamW(ptr ptr ptr)
9 stdcall AVIFileEndRecord(ptr)
10 stdcall AVIFileExit()
11 stdcall AVIFileGetStream(ptr ptr long long)
12 stdcall AVIFileInfo (ptr ptr long) AVIFileInfoA # A in both Win95 and NT
13 stdcall AVIFileInfoA(ptr ptr long)
14 stdcall AVIFileInfoW(ptr ptr long)
15 stdcall AVIFileInit()
16 stdcall AVIFileOpen(ptr str long ptr) AVIFileOpenA
17 stdcall AVIFileOpenA(ptr str long ptr)
18 stdcall AVIFileOpenW(ptr wstr long ptr)
19 stdcall AVIFileReadData(ptr long ptr ptr)
20 stdcall AVIFileRelease(ptr)
21 stdcall AVIFileWriteData(ptr long ptr long)
22 stdcall AVIGetFromClipboard(ptr)
23 stdcall AVIMakeCompressedStream(ptr ptr ptr ptr)
24 stdcall AVIMakeFileFromStreams(ptr long ptr)
25 stdcall AVIMakeStreamFromClipboard(long long ptr)
26 stdcall AVIPutFileOnClipboard(ptr)
27 varargs AVISave(str ptr ptr long ptr ptr) AVISaveA
28 varargs AVISaveA(str ptr ptr long ptr ptr)
29 stdcall AVISaveOptions(long long long ptr ptr)
30 stdcall AVISaveOptionsFree(long ptr)
31 stdcall AVISaveV(str ptr ptr long ptr ptr) AVISaveVA
32 stdcall AVISaveVA(str ptr ptr long ptr ptr)
33 stdcall AVISaveVW(wstr ptr ptr long ptr ptr)
34 varargs AVISaveW(wstr ptr ptr long ptr ptr)
35 stdcall AVIStreamAddRef(ptr)
36 stdcall AVIStreamBeginStreaming(ptr long long long)
37 stdcall AVIStreamCreate(ptr long long ptr)
38 stdcall AVIStreamEndStreaming(ptr)
39 stdcall AVIStreamFindSample(ptr long long)
40 stdcall AVIStreamGetFrame(ptr long)
41 stdcall AVIStreamGetFrameClose(ptr)
42 stdcall AVIStreamGetFrameOpen(ptr ptr)
43 stdcall AVIStreamInfo (ptr ptr long) AVIStreamInfoA
44 stdcall AVIStreamInfoA(ptr ptr long)
45 stdcall AVIStreamInfoW(ptr ptr long)
46 stdcall AVIStreamLength(ptr)
47 stdcall AVIStreamOpenFromFile (ptr str long long long ptr) AVIStreamOpenFromFileA
48 stdcall AVIStreamOpenFromFileA(ptr str long long long ptr)
49 stdcall AVIStreamOpenFromFileW(ptr wstr long long long ptr)
50 stdcall AVIStreamRead(ptr long long ptr long ptr ptr)
51 stdcall AVIStreamReadData(ptr long ptr ptr)
52 stdcall AVIStreamReadFormat(ptr long ptr ptr)
53 stdcall AVIStreamRelease(ptr)
54 stdcall AVIStreamSampleToTime(ptr long)
55 stdcall AVIStreamSetFormat(ptr long ptr long)
56 stdcall AVIStreamStart(ptr)
57 stdcall AVIStreamTimeToSample(ptr long)
58 stdcall AVIStreamWrite(ptr long long ptr long long ptr ptr)
59 stdcall AVIStreamWriteData(ptr long ptr long)
60 stdcall CreateEditableStream(ptr ptr)
@ stdcall -private DllCanUnloadNow()
@ stdcall -private DllGetClassObject(ptr ptr ptr)
63 stdcall EditStreamClone(ptr ptr)
64 stdcall EditStreamCopy(ptr ptr ptr ptr)
65 stdcall EditStreamCut(ptr ptr ptr ptr)
66 stdcall EditStreamPaste(ptr ptr ptr ptr long long)
67 stdcall EditStreamSetInfo(ptr ptr long) EditStreamSetInfoA
68 stdcall EditStreamSetInfoA(ptr ptr long)
69 stdcall EditStreamSetInfoW(ptr ptr long)
70 stdcall EditStreamSetName(ptr str) EditStreamSetNameA
71 stdcall EditStreamSetNameA(ptr str)
72 stdcall EditStreamSetNameW(ptr wstr)
73 extern IID_IAVIEditStream
74 extern IID_IAVIFile
75 extern IID_IAVIStream
76 extern IID_IGetFrame

# FIXME: Not in native
@ stdcall -private DllRegisterServer()
@ stdcall -private DllUnregisterServer()
