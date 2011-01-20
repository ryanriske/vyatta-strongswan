/*
 * Copyright (C) 2008 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "load_tester_ipsec.h"

#include <time.h>

typedef struct private_load_tester_ipsec_t private_load_tester_ipsec_t;

/**
 * Private variables and functions of kernel_pfkey class.
 */
struct private_load_tester_ipsec_t {
	/**
	 * Public interface.
	 */
	load_tester_ipsec_t public;

	/**
	 * faked SPI counter
	 */
	u_int32_t spi;
};

METHOD(kernel_ipsec_t, get_spi, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   protocol_id_t protocol, u_int32_t reqid, u_int32_t *spi)
{
	*spi = ++this->spi;
	return SUCCESS;
}

METHOD(kernel_ipsec_t, get_cpi, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   u_int32_t reqid, u_int16_t *cpi)
{
	return FAILED;
}

METHOD(kernel_ipsec_t, add_sa, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   u_int32_t spi, protocol_id_t protocol, u_int32_t reqid, mark_t mark,
	   lifetime_cfg_t *lifetime, u_int16_t enc_alg, chunk_t enc_key,
	   u_int16_t int_alg, chunk_t int_key, ipsec_mode_t mode, u_int16_t ipcomp,
	   u_int16_t cpi, bool encap, bool inbound, traffic_selector_t *src_ts,
	   traffic_selector_t *dst_ts)
{
	return SUCCESS;
}

METHOD(kernel_ipsec_t, update_sa, status_t,
	   private_load_tester_ipsec_t *this, u_int32_t spi, protocol_id_t protocol,
	   u_int16_t cpi, host_t *src, host_t *dst, host_t *new_src,
	   host_t *new_dst, bool encap, bool new_encap, mark_t mark)
{
	return SUCCESS;
}

METHOD(kernel_ipsec_t, query_sa, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   u_int32_t spi, protocol_id_t protocol, mark_t mark, u_int64_t *bytes)
{
	return NOT_SUPPORTED;
}

METHOD(kernel_ipsec_t, del_sa, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   u_int32_t spi, protocol_id_t protocol, u_int16_t cpi, mark_t mark)
{
	return SUCCESS;
}

METHOD(kernel_ipsec_t, add_policy, status_t,
	   private_load_tester_ipsec_t *this, host_t *src, host_t *dst,
	   traffic_selector_t *src_ts, traffic_selector_t *dst_ts,
	   policy_dir_t direction, u_int32_t spi, protocol_id_t protocol,
	   u_int32_t reqid, mark_t mark, ipsec_mode_t mode, u_int16_t ipcomp,
	   u_int16_t cpi, bool routed)
{
	return SUCCESS;
}

METHOD(kernel_ipsec_t, query_policy, status_t,
	   private_load_tester_ipsec_t *this, traffic_selector_t *src_ts,
	   traffic_selector_t *dst_ts, policy_dir_t direction, mark_t mark,
	   u_int32_t *use_time)
{
	*use_time = time_monotonic(NULL);
	return SUCCESS;
}

METHOD(kernel_ipsec_t, del_policy, status_t,
	   private_load_tester_ipsec_t *this, traffic_selector_t *src_ts,
	   traffic_selector_t *dst_ts, policy_dir_t direction, mark_t mark,
	   bool unrouted)
{
	return SUCCESS;
}

METHOD(kernel_ipsec_t, bypass_socket, bool,
	private_load_tester_ipsec_t *this, int fd, int family)
{
	return TRUE;
}

METHOD(kernel_ipsec_t, destroy, void,
	   private_load_tester_ipsec_t *this)
{
	free(this);
}

/*
 * Described in header.
 */
load_tester_ipsec_t *load_tester_ipsec_create()
{
	private_load_tester_ipsec_t *this;

	INIT(this,
		.public = {
			.interface = {
				.get_spi = _get_spi,
				.get_cpi = _get_cpi,
				.add_sa = _add_sa,
				.update_sa = _update_sa,
				.query_sa = _query_sa,
				.del_sa = _del_sa,
				.add_policy = _add_policy,
				.query_policy = _query_policy,
				.del_policy = _del_policy,
				.bypass_socket = _bypass_socket,
				.destroy = _destroy,
			},
		},
		.spi = 0,
	);

	return &this->public;
}
