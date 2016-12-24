#pragma once


void DXF_Stream( std::wofstream& os, DrawingObects& drawobj, DL_Dxf* dxf, DL_WriterA* dw );
void DXF_WriteFile( bfs::path& pathname,  DrawingObects& drawobj );
