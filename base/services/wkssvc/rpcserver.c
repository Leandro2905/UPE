/*
 *  ReactOS Services
 *  Copyright (C) 2015 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS Services
 * FILE:             base/services/wkssvc/rpcserver.c
 * PURPOSE:          Workstation service
 * PROGRAMMER:       Eric Kohl
 */

/* INCLUDES *****************************************************************/

#include "precomp.h"

#include "lmerr.h"

WINE_DEFAULT_DEBUG_CHANNEL(wkssvc);

/* FUNCTIONS *****************************************************************/

DWORD
WINAPI
RpcThreadRoutine(
    LPVOID lpParameter)
{
    RPC_STATUS Status;

    Status = RpcServerUseProtseqEpW(L"ncacn_np", 20, L"\\pipe\\wkssvc", NULL);
    if (Status != RPC_S_OK)
    {
        ERR("RpcServerUseProtseqEpW() failed (Status %lx)\n", Status);
        return 0;
    }

    Status = RpcServerRegisterIf(wkssvc_v1_0_s_ifspec, NULL, NULL);
    if (Status != RPC_S_OK)
    {
        ERR("RpcServerRegisterIf() failed (Status %lx)\n", Status);
        return 0;
    }

    Status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);
    if (Status != RPC_S_OK)
    {
        ERR("RpcServerListen() failed (Status %lx)\n", Status);
    }

    return 0;
}


void __RPC_FAR * __RPC_USER midl_user_allocate(SIZE_T len)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, len);
}


void __RPC_USER midl_user_free(void __RPC_FAR * ptr)
{
    HeapFree(GetProcessHeap(), 0, ptr);
}


/* Function 0 */
unsigned long
__stdcall
NetrWkstaGetInfo(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    unsigned long Level,
    LPWKSTA_INFO WkstaInfo)
{
    TRACE("NetrWkstaGetInfo level %lu\n", Level);

    return 0;
}


/* Function 1 */
unsigned long
__stdcall
NetrWkstaSetInfo(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    unsigned long Level,
    LPWKSTA_INFO WkstaInfo,
    unsigned long *ErrorParameter)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 2 */
unsigned long
__stdcall
NetrWkstaUserEnum(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    LPWKSTA_USER_ENUM_STRUCT UserInfo,
    unsigned long PreferredMaximumLength,
    unsigned long *TotalEntries,
    unsigned long *ResumeHandle)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 3 */
unsigned long
__stdcall
NetrWkstaUserGetInfo(
    WKSSVC_IDENTIFY_HANDLE Unused,
    unsigned long Level,
    LPWKSTA_USER_INFO UserInfo)
{
    FIXME("(%s, %d, %p)\n", debugstr_w(Unused), Level, UserInfo);

    UNIMPLEMENTED;
    return 0;
}


/* Function 4 */
unsigned long
__stdcall
NetrWkstaUserSetInfo (
    WKSSVC_IDENTIFY_HANDLE Unused,
    unsigned long Level,
    LPWKSTA_USER_INFO UserInfo,
    unsigned long *ErrorParameter)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 5 */
unsigned long
__stdcall
NetrWkstaTransportEnum(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    LPWKSTA_TRANSPORT_ENUM_STRUCT TransportInfo,
    unsigned long PreferredMaximumLength,
    unsigned long* TotalEntries,
    unsigned long *ResumeHandle)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 6 */
unsigned long
__stdcall
NetrWkstaTransportAdd(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    unsigned long Level,
    LPWKSTA_TRANSPORT_INFO_0 TransportInfo,
    unsigned long *ErrorParameter)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 7 */
unsigned long
__stdcall
NetrWkstaTransportDel(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    wchar_t *TransportName,
    unsigned long ForceLevel)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 8 */
unsigned long
__stdcall
NetrUseAdd(
    WKSSVC_IMPERSONATE_HANDLE ServerName,
    unsigned long Level,
    LPUSE_INFO InfoStruct,
    unsigned long *ErrorParameter)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 9 */
unsigned long
__stdcall
NetrUseGetInfo(
    WKSSVC_IMPERSONATE_HANDLE ServerName,
    wchar_t *UseName,
    unsigned long Level,
    LPUSE_INFO InfoStruct)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 10 */
unsigned long
__stdcall
NetrUseDel(
    WKSSVC_IMPERSONATE_HANDLE ServerName,
    wchar_t *UseName,
    unsigned long ForceLevel)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 11 */
unsigned long
__stdcall
NetrUseEnum(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    LPUSE_ENUM_STRUCT InfoStruct,
    unsigned long PreferredMaximumLength,
    unsigned long *TotalEntries,
    unsigned long *ResumeHandle)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 12 - Not used on wire */
unsigned long
__stdcall
NetrMessageBufferSend(void)
{
    TRACE("NetrMessageBufferSend()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 13 */
unsigned long
__stdcall
NetrWorkstationStatisticsGet(
    WKSSVC_IDENTIFY_HANDLE ServerName,
    wchar_t *ServiceName,
    unsigned long Level,
    unsigned long Options,
    LPSTAT_WORKSTATION_0 *Buffer)
{
    PSTAT_WORKSTATION_0 pStatBuffer;

    TRACE("NetrWorkstationStatisticsGet(%p %p %lu 0x%lx %p)\n",
          ServerName, ServiceName, Level, Options, Buffer);

    if (Level != 0)
        return ERROR_INVALID_LEVEL;

    if (Options != 0)
        return ERROR_INVALID_PARAMETER;

    pStatBuffer = midl_user_allocate(sizeof(STAT_WORKSTATION_0));
    if (pStatBuffer == NULL)
        return ERROR_NOT_ENOUGH_MEMORY;

    ZeroMemory(pStatBuffer, sizeof(STAT_WORKSTATION_0));

    // FIXME: Return the actual statistcs data!

    *Buffer = pStatBuffer;

    return NERR_Success;
}


/* Function 14 - Not used on wire */
unsigned long
__stdcall
NetrLogonDomainNameAdd(
    WKSSVC_IDENTIFY_HANDLE DomainName)
{
    TRACE("NetrLogonDomainNameAdd(%s)\n",
          debugstr_w(DomainName));
    return ERROR_NOT_SUPPORTED;
}


/* Function 15 - Not used on wire */
unsigned long
__stdcall
NetrLogonDomainNameDel(
    WKSSVC_IDENTIFY_HANDLE DomainName)
{
    TRACE("NetrLogonDomainNameDel(%s)\n",
          debugstr_w(DomainName));
    return ERROR_NOT_SUPPORTED;
}


/* Function 16 - Not used on wire */
unsigned long
__stdcall
NetrJoinDomain(void)
{
    TRACE("NetrJoinDomain()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 17 - Not used on wire */
unsigned long
__stdcall
NetrUnjoinDomain(void)
{
    TRACE("NetrUnjoinDomain()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 18 - Not used on wire */
unsigned long
__stdcall
NetrValidateName(void)
{
    TRACE("NetrValidateName()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 19 - Not used on wire */
unsigned long
__stdcall
NetrRenameMachineInDomain(void)
{
    TRACE("NetrRenameMachineInDomain()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 20 */
unsigned long
__stdcall
NetrGetJoinInformation(
    WKSSVC_IMPERSONATE_HANDLE ServerName,
    wchar_t **NameBuffer,
    PNETSETUP_JOIN_STATUS BufferType)
{
    TRACE("NetrGetJoinInformation(%p %p %p)\n",
          ServerName, NameBuffer, BufferType);

    if (NameBuffer == NULL)
        return ERROR_INVALID_PARAMETER;

    return NetpGetJoinInformation(NameBuffer,
                                  BufferType);
}


/* Function 21 - Not used on wire */
unsigned long
__stdcall
NetrGetJoinableOUs(void)
{
    TRACE("NetrGetJoinableOUs()\n");
    return ERROR_NOT_SUPPORTED;
}


/* Function 22 */
unsigned long
__stdcall
NetrJoinDomain2(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *DomainNameParam,
    wchar_t *MachineAccountOU,
    wchar_t *AccountName,
    PJOINPR_ENCRYPTED_USER_PASSWORD Password,
    unsigned long Options)
{
    NET_API_STATUS status;

    FIXME("NetrJoinDomain2(%p %S %S %S %S %p 0x%lx)\n",
          RpcBindingHandle, ServerName, DomainNameParam, MachineAccountOU,
          AccountName, Password, Options);

    if (DomainNameParam == NULL)
        return ERROR_INVALID_PARAMETER;

    if (Options & NETSETUP_JOIN_DOMAIN)
    {
        FIXME("NetrJoinDomain2: NETSETUP_JOIN_DOMAIN is not supported yet!\n");
        status = ERROR_CALL_NOT_IMPLEMENTED;
    }
    else
    {
        status = NetpJoinWorkgroup(DomainNameParam);
    }

    return status;
}


/* Function 23 */
unsigned long
__stdcall
NetrUnjoinDomain2(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *AccountName,
    PJOINPR_ENCRYPTED_USER_PASSWORD Password,
    unsigned long Options)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 24 */
unsigned long
__stdcall
NetrRenameMachineInDomain2(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *MachineName,
    wchar_t *AccountName,
    PJOINPR_ENCRYPTED_USER_PASSWORD Password,
    unsigned long Options)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 25 */
unsigned long
__stdcall
NetrValidateName2(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *NameToValidate,
    wchar_t *AccountName,
    PJOINPR_ENCRYPTED_USER_PASSWORD Password,
    NETSETUP_NAME_TYPE NameType)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 26 */
unsigned long
__stdcall
NetrGetJoinableOUs2(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *DomainNameParam,
    wchar_t *AccountName,
    PJOINPR_ENCRYPTED_USER_PASSWORD Password,
    unsigned long* OUCount,
    wchar_t ***OUs)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 27 */
unsigned long
__stdcall
NetrAddAlternateComputerName(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *AlternateName,
    wchar_t *DomainAccount,
    PJOINPR_ENCRYPTED_USER_PASSWORD EncryptedPassword,
    unsigned long Reserved)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 28 */
unsigned long
__stdcall
NetrRemoveAlternateComputerName(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *AlternateName,
    wchar_t *DomainAccount,
    PJOINPR_ENCRYPTED_USER_PASSWORD EncryptedPassword,
    unsigned long Reserved)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 29 */
unsigned long
__stdcall
NetrSetPrimaryComputerName(
    handle_t RpcBindingHandle,
    wchar_t *ServerName,
    wchar_t *PrimaryName,
    wchar_t *DomainAccount,
    PJOINPR_ENCRYPTED_USER_PASSWORD EncryptedPassword,
    unsigned long Reserved)
{
    UNIMPLEMENTED;
    return 0;
}


/* Function 30 */
unsigned long
__stdcall
NetrEnumerateComputerNames(
    WKSSVC_IMPERSONATE_HANDLE ServerName,
    NET_COMPUTER_NAME_TYPE NameType,
    unsigned long Reserved,
    PNET_COMPUTER_NAME_ARRAY *ComputerNames)
{
    UNIMPLEMENTED;
    return 0;
}

/* EOF */
