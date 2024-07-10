
## CFLAG OPT
MCFLAG_OPT   	= --c99 -DOS_USING_ONEOS_SOCKET -DOS_USING_LIBC_ADAPTER

## user config && TCP_TEST
MINCPATH += Third-party/oneos/cms/src \
            Third-party/oneos/cms/src/test \
            Third-party/oneos/ \
            external/oneos/inc/kernel \
            external/oneos/inc/

#MSRCPATH += Third-party/oneos/cms/user
#MSRCPATH += Third-party/oneos/cms/user/test
#SOURCES += cms_config.c cms_tcp_test.c cms_mqtt_test.c

## common
MINCPATH += Third-party/oneos/cms/cms_core/common \
            Third-party/oneos/cms/cms_core/port \
            Third-party/oneos/cms/cms_core/port/oneos

MSRCPATH += Third-party/oneos/cms/cms_core/common \
            Third-party/oneos/cms/cms_core/port/oneos

SOURCES += cms_log.c cms_timer.c cms_clock.c cms_memory.c cms_mutex.c cms_net.c cms_ring_buff.c cms_sem.c cms_thread.c

#######################################################################################
#####    cms_connect
#######################################################################################

MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/include

### common
MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/common
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/common
SOURCES += cms_authentication.c

### cms_protocol
MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/cms_protocol

MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/cms_protocol
SOURCES += cms_con_protocol_tcp.c cms_con_protocol_udp.c

MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/cms_protocol/src
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/cms_protocol/src
SOURCES += cms_con_protocol.c cms_con_protocol_list.c cms_con_heartbeat.c

### connect_manager
MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/connect_manager
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/connect_manager
SOURCES += cms_con_manager_tcp.c cms_con_manager_udp.c

### connect_server
MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/connect_server
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/connect_server
SOURCES += cms_con_server_tcp.c cms_con_server_udp.c

### mqtt && tcp
MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/mqtt
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/mqtt
SOURCES += cms_con_mqtt.c cms_con_mqtt_core.c cms_con_mqtt_list.c

MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/mqtt/src
MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/mqtt/src
SOURCES += cms_con_mqtt_publish.c cms_con_mqtt_packet.c cms_con_mqtt_connect.c cms_con_mqtt_subscribe.c cms_con_mqtt_unsubscribe.c

MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/tcp
SOURCES += cms_con_tcp.c

### coap && udp && lwm2m
#MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/coap
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/coap
#SOURCES += cms_con_coap.c cms_con_coap_core.c cms_con_coap_option.c cms_con_coap_packet.c

MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/udp
SOURCES += cms_con_udp.c

#MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/include
#MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/include/cms-impl
#MINCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/include/er-coap-13
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/src/cms_impl
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/src/cms_impl/client
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/src/cms_impl/shared
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/src/core
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_connect/src/lwm2m/src/er-coap-13
#SOURCES  += lwm2mclient.c object_access_control.c object_connectivity_moni.c object_connectivity_stat.c \
#			object_device.c object_firmware.c object_location.c object_security.c object_server.c system_api.c \
#			test_object.c commandline.c memtrace.c platform.c mbedconnection.c connection.c \
#			cms_lwm2m_api.c cms_lwm2m_net.c cms_lwm2m_sys.c cms_object.c data.c utils.c uri.c \
#			liblwm2m.c transaction.c registration.c list.c tlv.c packet.c block1.c objects.c \
#			discover.c observe.c management.c json.c bootstrap.c er-coap-13.c

#######################################################################################
#####    cms_id
#######################################################################################

MINCPATH += Third-party/oneos/cms/cms_core/cms_id/include

MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src
SOURCES += cms_id_internal.c cms_id_store_interface.c

### cbor
MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/cbor
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/cbor
SOURCES += cbor_decode.c cbor_encode.c cbor_util.c ieee754.c UsefulBuf.c

### cose
MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/cose
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/cose
SOURCES += cose_crypto.c cose_mac0_sign.c cose_mac0_verify.c cose_parameters.c cose_util.c

### mgt
MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/mgt
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/mgt
SOURCES += id_mgt.c id_mgt_con.c
#SOURCES += id_mgt.c id_mgt_udp.c

### pbkdf2
#MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/pbkdf2
#MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/pbkdf2
#SOURCES += pbkdf2.c

### sha
MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/sha
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/sha
SOURCES += sha.c sha_hkdf.c sha_hmac.c sha224-256.c
#SOURCES += sha.c sha_hkdf.c sha_hmac.c sha1.c sha224-256.c sha384-512.c

### attestation
MINCPATH += Third-party/oneos/cms/cms_core/cms_id/src/attestation
MSRCPATH += Third-party/oneos/cms/cms_core/cms_id/src/attestation
SOURCES += attest_core.c attest_symmetric.c attest_token_encode.c
