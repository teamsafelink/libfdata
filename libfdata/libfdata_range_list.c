/*
 * The range list functions
 *
 * Copyright (c) 2010-2013, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfdata_definitions.h"
#include "libfdata_libcdata.h"
#include "libfdata_libcerror.h"
#include "libfdata_libfcache.h"
#include "libfdata_list.h"
#include "libfdata_range_list.h"
#include "libfdata_types.h"

/* Creates a range list
 * Make sure the value range_list is referencing, is set to NULL
 *
 * If the flag LIBFDATA_FLAG_DATA_HANDLE_MANAGED is set the range list
 * takes over management of the data handle and the data handle is freed when
 * no longer needed
 *
 * Returns 1 if successful or -1 on error
 */
int libfdata_range_list_initialize(
     libfdata_range_list_t **range_list,
     intptr_t *data_handle,
     int (*free_data_handle)(
            intptr_t **data_handle,
            libcerror_error_t **error ),
     int (*clone_data_handle)(
            intptr_t **destination_data_handle,
            intptr_t *source_data_handle,
            libcerror_error_t **error ),
     int (*read_element_data)(
            intptr_t *data_handle,
            intptr_t *file_io_handle,
            libfdata_list_element_t *list_element,
            libfcache_cache_t *cache,
            int element_file_index,
            off64_t element_offset,
            size64_t element_size,
            uint32_t element_flags,
            uint8_t read_flags,
            libcerror_error_t **error ),
     int (*write_element_data)(
            intptr_t *data_handle,
            intptr_t *file_io_handle,
            libfdata_list_element_t *list_element,
            libfcache_cache_t *cache,
            int element_file_index,
            off64_t element_offset,
            size64_t element_size,
            uint32_t element_flags,
            uint8_t write_flags,
            libcerror_error_t **error ),
     uint8_t flags,
     libcerror_error_t **error )
{
	libfdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libfdata_range_list_initialize";

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid range list value already set.",
		 function );

		return( -1 );
	}
	if( ( flags & 0xfe ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function );

		return( -1 );
	}
	internal_range_list = memory_allocate_structure(
	                       libfdata_internal_range_list_t );

	if( internal_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create range list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_range_list,
	     0,
	     sizeof( libfdata_internal_range_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear range list.",
		 function );

		memory_free(
		 internal_range_list );

		return( -1 );
	}
	if( libcdata_range_list_initialize(
	     &( internal_range_list->elements_range_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create elements range list.",
		 function );

		goto on_error;
	}
	internal_range_list->flags             |= flags;
	internal_range_list->data_handle        = data_handle;
	internal_range_list->free_data_handle   = free_data_handle;
	internal_range_list->clone_data_handle  = clone_data_handle;
	internal_range_list->read_element_data  = read_element_data;
	internal_range_list->write_element_data = write_element_data;

	*range_list = (libfdata_range_list_t *) internal_range_list;

	return( 1 );

on_error:
	if( internal_range_list != NULL )
	{
		if( internal_range_list->elements_range_list != NULL )
		{
			libcdata_range_list_free(
			 &( internal_range_list->elements_range_list ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_range_list );
	}
	return( -1 );
}

/* Frees a range list
 * Returns 1 if successful or -1 on error
 */
int libfdata_range_list_free(
     libfdata_range_list_t **range_list,
     libcerror_error_t **error )
{
	libfdata_internal_range_list_t *internal_range_list = NULL;
	static char *function                               = "libfdata_range_list_free";
	int result                                          = 1;

	if( range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *range_list != NULL )
	{
		internal_range_list = (libfdata_internal_range_list_t *) *range_list;
		*range_list         = NULL;

		if( libcdata_range_list_free(
		     &( internal_range_list->elements_range_list ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfdata_list_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the elements range list.",
			 function );

			result = -1;
		}
		if( ( internal_range_list->flags & LIBFDATA_FLAG_DATA_HANDLE_MANAGED ) != 0 )
		{
			if( internal_range_list->data_handle != NULL )
			{
				if( internal_range_list->free_data_handle == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: invalid range list - missing free data handle function.",
					 function );

					result = -1;
				}
				else if( internal_range_list->free_data_handle(
				          &( internal_range_list->data_handle ),
				          error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free data handle.",
					 function );

					result = -1;
				}
			}
		}
		memory_free(
		 internal_range_list );
	}
	return( result );
}

/* Clones (duplicates) the range list
 * Returns 1 if successful or -1 on error
 */
int libfdata_range_list_clone(
     libfdata_range_list_t **destination_range_list,
     libfdata_range_list_t *source_range_list,
     libcerror_error_t **error )
{
	libfdata_internal_range_list_t *internal_destination_range_list = NULL;
	libfdata_internal_range_list_t *internal_source_range_list      = NULL;
	static char *function                                           = "libfdata_range_list_clone";

	if( destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination range list.",
		 function );

		return( -1 );
	}
	if( *destination_range_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination range list value already set.",
		 function );

		return( -1 );
	}
	if( source_range_list == NULL )
	{
		*destination_range_list = NULL;

		return( 1 );
	}
	internal_source_range_list = (libfdata_internal_range_list_t *) source_range_list;

	internal_destination_range_list = memory_allocate_structure(
	                                   libfdata_internal_range_list_t );

	if( internal_destination_range_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination range list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_destination_range_list,
	     0,
	     sizeof( libfdata_internal_range_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear destination range list.",
		 function );

		memory_free(
		 internal_destination_range_list );

		return( -1 );
	}
	if( internal_source_range_list->data_handle != NULL )
	{
		if( internal_source_range_list->free_data_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source range list - missing free data handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_range_list->clone_data_handle == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid source range list - missing clone data handle function.",
			 function );

			goto on_error;
		}
		if( internal_source_range_list->clone_data_handle(
		     &( internal_destination_range_list->data_handle ),
		     internal_source_range_list->data_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination data handle.",
			 function );

			goto on_error;
		}
	}
	if( libcdata_range_list_clone(
	     &( internal_destination_range_list->elements_range_list ),
	     internal_source_range_list->elements_range_list,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfdata_list_free,
	     (int (*)(intptr_t **, intptr_t *, libcerror_error_t **)) &libfdata_list_clone,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination elements range list.",
		 function );

		goto on_error;
	}
	internal_destination_range_list->flags              = internal_source_range_list->flags | LIBFDATA_FLAG_DATA_HANDLE_MANAGED;
	internal_destination_range_list->free_data_handle   = internal_source_range_list->free_data_handle;
	internal_destination_range_list->clone_data_handle  = internal_source_range_list->clone_data_handle;
	internal_destination_range_list->read_element_data  = internal_source_range_list->read_element_data;
	internal_destination_range_list->write_element_data = internal_source_range_list->write_element_data;

	*destination_range_list == (libfdata_range_list_t *) internal_destination_range_list;

	return( 1 );

on_error:
	if( internal_destination_range_list != NULL )
	{
		if( internal_destination_range_list->data_handle != NULL )
		{
			internal_source_range_list->free_data_handle(
			 &( internal_destination_range_list->data_handle ),
			 NULL );
		}
		memory_free(
		 internal_destination_range_list );
	}
	return( -1 );
}

