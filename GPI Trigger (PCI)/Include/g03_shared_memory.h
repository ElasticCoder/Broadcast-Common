///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1996; Sony BPE
// All rights Reserved
//
///////////////////////////////////////////////////////////////////////////////
//
// System Name : Key And Voice-Over
// Subsystem   : Services
// File Name   : g03_shared_memory.h       
// File Type   : C++ Header
// Description : Memory Shared By Several Processes Class
// Author      : Duncan Olesen
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _G03_SHARED_MEMORY_H
#define _G03_SHARED_MEMORY_H

#include <portable_types.h>  // Type definitions
#include <G00.h>        // Services DLL import/export control
#include <g03_types.h>  // ...

class SERVICES_EXTERNAL C_g03_shared_memory
{
public:
    C_g03_shared_memory();
    C_g03_shared_memory(const tc *p_memory_name_tcp);
    ~C_g03_shared_memory();

// Attributes
private:
    HANDLE m_file_mapping_h;
    void   *m_memory_address_p;
    m      m_size_m;

// Implementation
public:
	void*           configure(const tc* p_memory_name_tcp,
                              m         p_size_m);          // Configure the shared memory
	void            attach(const tc *p_memory_name_tcp);    // Attach to the shared memory
    inline void*    address() const
    {
        return m_memory_address_p;
    };                                                     // Get shared memory address
    T_g03_memory_id	memory_id(const void* p_mem_p) const;  // Get shared memory id
    void*           address(T_g03_memory_id p_memory_id) const;
														   // Get shared memory address
    const tc*       name() const;                          // Get shared memory name
};

#endif  // _G03_SHARED_MEMORY_H

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
