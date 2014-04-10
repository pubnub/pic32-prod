#include <stdbool.h>

#include "pubnub.h"


#if PUBNUB_SSL
static const unsigned char ssl_cert[] = {
    0x30, 0x82, 0x05, 0x80, 0x30, 0x82, 0x04, 0x68, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x07, 0x4e, 0xf9, 0x1b, 0x13, 0x07, 0xd5, 0x59, 0x30, 0x0d,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05,
    0x00, 0x30, 0x81, 0xdc, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55,
    0x04, 0x08, 0x13, 0x07, 0x41, 0x72, 0x69, 0x7a, 0x6f, 0x6e, 0x61, 0x31,
    0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x0a, 0x53, 0x63,
    0x6f, 0x74, 0x74, 0x73, 0x64, 0x61, 0x6c, 0x65, 0x31, 0x25, 0x30, 0x23,
    0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x1c, 0x53, 0x74, 0x61, 0x72, 0x66,
    0x69, 0x65, 0x6c, 0x64, 0x20, 0x54, 0x65, 0x63, 0x68, 0x6e, 0x6f, 0x6c,
    0x6f, 0x67, 0x69, 0x65, 0x73, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31,
    0x39, 0x30, 0x37, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x13, 0x30, 0x68, 0x74,
    0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69,
    0x63, 0x61, 0x74, 0x65, 0x73, 0x2e, 0x73, 0x74, 0x61, 0x72, 0x66, 0x69,
    0x65, 0x6c, 0x64, 0x74, 0x65, 0x63, 0x68, 0x2e, 0x63, 0x6f, 0x6d, 0x2f,
    0x72, 0x65, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x6f, 0x72, 0x79, 0x31, 0x31,
    0x30, 0x2f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x28, 0x53, 0x74, 0x61,
    0x72, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x20, 0x53, 0x65, 0x63, 0x75, 0x72,
    0x65, 0x20, 0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
    0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72, 0x69, 0x74,
    0x79, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x05, 0x13, 0x08,
    0x31, 0x30, 0x36, 0x38, 0x38, 0x34, 0x33, 0x35, 0x30, 0x1e, 0x17, 0x0d,
    0x31, 0x32, 0x30, 0x39, 0x31, 0x32, 0x32, 0x31, 0x33, 0x37, 0x34, 0x33,
    0x5a, 0x17, 0x0d, 0x31, 0x37, 0x30, 0x39, 0x31, 0x31, 0x31, 0x39, 0x32,
    0x39, 0x31, 0x30, 0x5a, 0x30, 0x51, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0c, 0x2a, 0x2e, 0x70, 0x75, 0x62, 0x6e, 0x75,
    0x62, 0x2e, 0x63, 0x6f, 0x6d, 0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55,
    0x04, 0x0b, 0x13, 0x18, 0x44, 0x6f, 0x6d, 0x61, 0x69, 0x6e, 0x20, 0x43,
    0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x20, 0x56, 0x61, 0x6c, 0x69, 0x64,
    0x61, 0x74, 0x65, 0x64, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0c, 0x0c, 0x2a, 0x2e, 0x70, 0x75, 0x62, 0x6e, 0x75, 0x62, 0x2e,
    0x63, 0x6f, 0x6d, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82,
    0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00,
    0xd0, 0x7a, 0x12, 0x94, 0x7c, 0x06, 0xcc, 0x9f, 0xd1, 0xfa, 0x71, 0x1b,
    0x3f, 0x5d, 0x42, 0x10, 0xa9, 0xe6, 0x5e, 0xf0, 0xbb, 0xf3, 0xa6, 0xe5,
    0xaf, 0xca, 0x20, 0xad, 0x14, 0x29, 0xb8, 0xb2, 0x6f, 0xbd, 0x49, 0xec,
    0xa2, 0x21, 0xc0, 0xc2, 0x96, 0x1b, 0xf7, 0x19, 0xf9, 0x1e, 0x88, 0xa0,
    0x0e, 0xda, 0x57, 0x6b, 0xb4, 0x32, 0x27, 0x00, 0xa5, 0x6c, 0x16, 0xb1,
    0x35, 0x56, 0xb0, 0xd8, 0x93, 0x12, 0xfb, 0x7c, 0x8f, 0x4a, 0x5d, 0x6c,
    0x2c, 0xf5, 0x82, 0x84, 0xca, 0xd4, 0x87, 0x87, 0x6b, 0xaa, 0x21, 0x6c,
    0x4d, 0xf5, 0x96, 0x1c, 0xc9, 0x8a, 0x32, 0x00, 0x2c, 0x01, 0xf8, 0xe9,
    0x0b, 0x23, 0x48, 0x9e, 0x34, 0xbf, 0x27, 0xd7, 0x7f, 0xa7, 0x95, 0xc1,
    0xc2, 0x28, 0x89, 0x92, 0x5e, 0x50, 0x1a, 0xe1, 0x3b, 0x58, 0xe5, 0xd8,
    0x88, 0xe3, 0xf1, 0x38, 0x30, 0x85, 0xf5, 0x93, 0x43, 0x4c, 0x69, 0x9e,
    0x32, 0x8d, 0xa8, 0xf5, 0xc4, 0x89, 0x8a, 0xcd, 0xba, 0xc2, 0xc2, 0x87,
    0x33, 0x1b, 0xfe, 0xa0, 0x0f, 0x29, 0x52, 0x7a, 0x01, 0x54, 0xac, 0xe6,
    0x29, 0xa4, 0xfe, 0x0c, 0xe7, 0x8d, 0x64, 0x7b, 0x52, 0x38, 0x81, 0x28,
    0x53, 0x71, 0x8a, 0x6a, 0xc3, 0xe7, 0xa2, 0xd6, 0x47, 0x11, 0x80, 0x3a,
    0x38, 0xcb, 0x57, 0xa3, 0x2e, 0xde, 0x88, 0x86, 0x18, 0x08, 0x1b, 0x53,
    0x9f, 0xc8, 0xd1, 0xca, 0x4a, 0xaa, 0xdb, 0x74, 0x31, 0x77, 0x7e, 0x1c,
    0xbd, 0x65, 0x2c, 0x24, 0xef, 0xc5, 0x83, 0x17, 0x45, 0xab, 0x37, 0xa3,
    0x4c, 0x30, 0xb7, 0x90, 0x13, 0x06, 0x64, 0x0e, 0xf2, 0xd9, 0x7a, 0xb1,
    0x16, 0x1c, 0xc3, 0xc6, 0x7e, 0x5b, 0x26, 0x07, 0x77, 0x20, 0x73, 0xe8,
    0x7d, 0x5f, 0x79, 0x95, 0x45, 0x82, 0x55, 0x45, 0x02, 0x51, 0x39, 0x95,
    0xdf, 0xf7, 0xed, 0x8f, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01,
    0xcf, 0x30, 0x82, 0x01, 0xcb, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x1d, 0x13,
    0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0x00, 0x30, 0x1d,
    0x06, 0x03, 0x55, 0x1d, 0x25, 0x04, 0x16, 0x30, 0x14, 0x06, 0x08, 0x2b,
    0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x06, 0x08, 0x2b, 0x06, 0x01,
    0x05, 0x05, 0x07, 0x03, 0x02, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x1d, 0x0f,
    0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x05, 0xa0, 0x30, 0x39, 0x06,
    0x03, 0x55, 0x1d, 0x1f, 0x04, 0x32, 0x30, 0x30, 0x30, 0x2e, 0xa0, 0x2c,
    0xa0, 0x2a, 0x86, 0x28, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x63,
    0x72, 0x6c, 0x2e, 0x73, 0x74, 0x61, 0x72, 0x66, 0x69, 0x65, 0x6c, 0x64,
    0x74, 0x65, 0x63, 0x68, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x73, 0x66, 0x73,
    0x31, 0x2d, 0x32, 0x33, 0x2e, 0x63, 0x72, 0x6c, 0x30, 0x59, 0x06, 0x03,
    0x55, 0x1d, 0x20, 0x04, 0x52, 0x30, 0x50, 0x30, 0x4e, 0x06, 0x0b, 0x60,
    0x86, 0x48, 0x01, 0x86, 0xfd, 0x6e, 0x01, 0x07, 0x17, 0x01, 0x30, 0x3f,
    0x30, 0x3d, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x02, 0x01,
    0x16, 0x31, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x63, 0x65, 0x72,
    0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x73, 0x2e, 0x73, 0x74,
    0x61, 0x72, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x74, 0x65, 0x63, 0x68, 0x2e,
    0x63, 0x6f, 0x6d, 0x2f, 0x72, 0x65, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x6f,
    0x72, 0x79, 0x2f, 0x30, 0x81, 0x8d, 0x06, 0x08, 0x2b, 0x06, 0x01, 0x05,
    0x05, 0x07, 0x01, 0x01, 0x04, 0x81, 0x80, 0x30, 0x7e, 0x30, 0x2a, 0x06,
    0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07, 0x30, 0x01, 0x86, 0x1e, 0x68,
    0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x6f, 0x63, 0x73, 0x70, 0x2e, 0x73,
    0x74, 0x61, 0x72, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x74, 0x65, 0x63, 0x68,
    0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x30, 0x50, 0x06, 0x08, 0x2b, 0x06, 0x01,
    0x05, 0x05, 0x07, 0x30, 0x02, 0x86, 0x44, 0x68, 0x74, 0x74, 0x70, 0x3a,
    0x2f, 0x2f, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
    0x65, 0x73, 0x2e, 0x73, 0x74, 0x61, 0x72, 0x66, 0x69, 0x65, 0x6c, 0x64,
    0x74, 0x65, 0x63, 0x68, 0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x72, 0x65, 0x70,
    0x6f, 0x73, 0x69, 0x74, 0x6f, 0x72, 0x79, 0x2f, 0x73, 0x66, 0x5f, 0x69,
    0x6e, 0x74, 0x65, 0x72, 0x6d, 0x65, 0x64, 0x69, 0x61, 0x74, 0x65, 0x2e,
    0x63, 0x72, 0x74, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18,
    0x30, 0x16, 0x80, 0x14, 0x49, 0x4b, 0x52, 0x27, 0xd1, 0x1b, 0xbc, 0xf2,
    0xa1, 0x21, 0x6a, 0x62, 0x7b, 0x51, 0x42, 0x7a, 0x8a, 0xd7, 0xd5, 0x56,
    0x30, 0x23, 0x06, 0x03, 0x55, 0x1d, 0x11, 0x04, 0x1c, 0x30, 0x1a, 0x82,
    0x0c, 0x2a, 0x2e, 0x70, 0x75, 0x62, 0x6e, 0x75, 0x62, 0x2e, 0x63, 0x6f,
    0x6d, 0x82, 0x0a, 0x70, 0x75, 0x62, 0x6e, 0x75, 0x62, 0x2e, 0x63, 0x6f,
    0x6d, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14,
    0xa4, 0xf8, 0x3f, 0xdf, 0xbe, 0x88, 0xc9, 0xed, 0x3e, 0x5d, 0x90, 0x99,
    0x9e, 0x3f, 0x54, 0x0d, 0x87, 0x91, 0x29, 0xd1, 0x30, 0x0d, 0x06, 0x09,
    0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03,
    0x82, 0x01, 0x01, 0x00, 0x2e, 0x71, 0xca, 0x41, 0xeb, 0x33, 0x97, 0x0e,
    0x30, 0xf3, 0x8e, 0xa1, 0x7e, 0x30, 0xf7, 0x8f, 0xe7, 0x65, 0xf5, 0x6c,
    0x92, 0x6b, 0xd6, 0xa9, 0x65, 0xcb, 0x5a, 0x45, 0xe1, 0xb9, 0x18, 0x98,
    0xc9, 0x94, 0x2a, 0x4a, 0xa9, 0x16, 0xdc, 0x17, 0xa9, 0xc0, 0x71, 0xa6,
    0x54, 0x83, 0x55, 0x79, 0xce, 0x16, 0x08, 0x90, 0x9c, 0xc8, 0x08, 0x55,
    0x45, 0xcc, 0x0f, 0x44, 0x4e, 0xd0, 0x2f, 0x4b, 0xcb, 0xf4, 0x29, 0x52,
    0xcf, 0xe2, 0xce, 0x55, 0xd1, 0xc9, 0x81, 0xaa, 0x59, 0xb7, 0x9c, 0xcf,
    0x78, 0x5f, 0x5a, 0x74, 0xef, 0xe6, 0x30, 0x99, 0x50, 0xe5, 0x44, 0x4c,
    0xa4, 0xc1, 0x8b, 0x4c, 0x33, 0x78, 0xe3, 0x8f, 0xec, 0xd4, 0x89, 0xc4,
    0x7a, 0x16, 0xc1, 0x6e, 0xa6, 0x9b, 0x2d, 0x2e, 0x30, 0x05, 0xee, 0xf6,
    0x55, 0xb2, 0x39, 0xb0, 0xea, 0x12, 0x8e, 0x9f, 0x5e, 0x28, 0x19, 0x18,
    0xa6, 0x72, 0x37, 0x87, 0xe7, 0xd4, 0x3f, 0x55, 0x62, 0x4b, 0xb6, 0xda,
    0x2b, 0x01, 0xce, 0xe6, 0xcc, 0x01, 0xcf, 0x92, 0x90, 0x44, 0x02, 0xed,
    0xe8, 0x73, 0x9d, 0x42, 0xe3, 0xda, 0xdb, 0x38, 0x4f, 0xbd, 0xcd, 0x89,
    0xcf, 0x63, 0xda, 0xfe, 0x8a, 0x40, 0x4c, 0x74, 0x9d, 0xdb, 0x4a, 0xdb,
    0xbf, 0x69, 0x00, 0xbf, 0xeb, 0xc4, 0xaa, 0x84, 0x53, 0x07, 0x00, 0x8f,
    0xe3, 0x6a, 0x25, 0x99, 0xbd, 0x40, 0x0e, 0x68, 0xec, 0x39, 0x3b, 0x80,
    0x0a, 0x3c, 0xf4, 0x0f, 0x0b, 0x67, 0x43, 0xbe, 0xc6, 0x9b, 0x2e, 0xc3,
    0x68, 0x9b, 0x57, 0xbb, 0xfa, 0xeb, 0x6b, 0x7b, 0x51, 0x75, 0x1b, 0x48,
    0x38, 0x63, 0xab, 0x10, 0x3c, 0x3d, 0x48, 0xfc, 0x97, 0xca, 0x14, 0xc3,
    0xe7, 0xae, 0x66, 0x49, 0x73, 0x3e, 0xaf, 0xc0, 0xac, 0xb7, 0xeb, 0xa3,
    0x07, 0x0f, 0x1d, 0x55, 0x64, 0xf0, 0x32, 0xad,
};

/* Custom recv/send callbacks for CyaSSL, as used in GenericCyaSSLClient. */

static int ssl_recvf(CYASSL* ssl, char* buf, int sz, void* vp) {
    int sockfd = *(int*) vp;
    if (TCPIsGetReady(sockfd)) {
        WORD got = TCPGetArray(sockfd, (BYTE*) buf, sz);
        return got;
    } else
        return -2; // WANT_READ
}

static int ssl_sendf(CYASSL* ssl, char* buf, int sz, void* vp) {
    int sockfd = *(int*) vp;
    if (TCPIsPutReady(sockfd)) {
        WORD put = TCPPutArray(sockfd, (BYTE*) buf, sz);
        return put;
    } else
        return -2; // WANT_WRITE
}

/* All pubnub contexts share a common CyaSSL context. */
CYASSL_CTX *ssl_ctx;
static CYASSL_CTX *
get_ssl_ctx()
{
    if (!ssl_ctx) {
        ssl_ctx = CyaSSL_CTX_new(CyaTLSv1_2_client_method());
        CyaSSL_CTX_load_verify_buffer(ssl_ctx, ssl_cert,
                sizeof(ssl_cert), SSL_FILETYPE_ASN1);

        CyaSSL_SetIORecv(ssl_ctx, ssl_recvf);
        CyaSSL_SetIOSend(ssl_ctx, ssl_sendf);
    }
    return ssl_ctx;
}

static int
pubnub_tcp_read(struct pubnub *p, BYTE *buf, size_t buf_size, size_t buf_pos)
{
    return CyaSSL_read(p->ssl, buf + buf_size, buf_pos - buf_size);
}

static bool
pubnub_tcp_writestr(struct pubnub *p, char *string, int len)
{
    int r = CyaSSL_write(p->ssl, string, len);
    if (r == SSL_ERROR_WANT_READ || r == SSL_ERROR_WANT_WRITE)
        return false;
    return true;
}

#else

static ssize_t
pubnub_tcp_read(struct pubnub *p, BYTE *buf, size_t buf_size, size_t buf_pos)
{
    return TCPGetArray(p->socket, buf + buf_size, buf_pos - buf_size);
}

static bool
pubnub_tcp_writestr(struct pubnub *p, char *string, int len)
{
    if (TCPIsPutReady(p->socket) < len)
        return false;
    TCPPutString(p->socket, string);
    return true;
}
#endif


/* Find the beginning of a JSON string that ends at &buf[len].
 * Return -1 on error. */
static int
extract_last_string(char *buf, int len)
{
    buf[len] = 0;
    int i;
    for (i = len-1; i > 0; i--)
        if (buf[i] == '"')
            break;
    if (!i || buf[i-1] != ',')
        return -1;
    return i;
}

/* Split a JSON array (with arbitrary contents) to multiple NUL-terminated
 * C strings. */
static bool
split_array(char *buf, int len)
{
    bool escaped = false, in_string = false;
    int bracket_level = 0;
    int i;
    for (i = 0; i < len; i++) {
        if (escaped) {
            escaped = false;
        } else if (in_string) {
            switch (buf[i]) {
                case '\\': escaped = true; break;
                case '"': in_string = false; break;
                default: break;
            }
        } else {
            switch (buf[i]) {
                case '"': in_string = true; break;
                case '[': case '{': bracket_level++; break;
                case ']': case '}': bracket_level--; break;
                /* if at root, split! */
                case ',': if (bracket_level == 0) buf[i] = 0; break;
                default: break;
            }
        }
    }
    if (escaped || in_string || bracket_level > 0)
        return false;
    return true;
}


void
pubnub_init(struct pubnub *p, const char *publish_key, const char *subscribe_key)
{
    p->publish_key = publish_key;
    p->subscribe_key = subscribe_key;
    p->timeout = 10;
    p->sub_timeout = 310;
#if PUBNUB_SSL
    pubnub_set_origin(p, "https://pubsub.pubnub.com/");
#else
    pubnub_set_origin(p, "http://pubsub.pubnub.com/");
#endif
    strcpy(p->timetoken, "0");
    p->uuid = NULL;
    p->auth = NULL;
}

void
pubnub_done(struct pubnub *p)
{
    if (p->state != PS_IDLE) {
        TCPDisconnect(p->socket);
#if PUBNUB_SSL
        if (p->ssl) {
            CyaSSL_shutdown(p->ssl);
            CyaSSL_free(p->ssl);
            p->ssl = NULL;
        }
#endif
    }
    if (p->http_reply) {
        free(p->http_reply); p->http_reply = NULL;
    }
}

void
pubnub_set_origin(struct pubnub *p, const char *origin)
{
#if PUBNUB_SSL
    if (origin[4] == 's') {
        p->use_ssl = 1;
        strcpy(p->origin, origin + strlen("https://"));
    } else
#endif
    {
        strcpy(p->origin, origin + strlen("http://"));
    }
    int namelen = strlen(p->origin);
    if (p->origin[namelen-1] == '/')
        p->origin[namelen-1] = 0;
}

void
pubnub_set_timeout(struct pubnub *p, int timeout)
{
    p->timeout = timeout;
}

void
pubnub_set_sub_timeout(struct pubnub *p, int timeout)
{
    p->sub_timeout = timeout;
}

void
pubnub_set_uuid(struct pubnub *p, const char *uuid)
{
    p->uuid = uuid;
}

void
pubnub_set_auth(struct pubnub *p, const char *auth)
{
    p->auth = auth;
}


static bool
pubnub_http_connect(struct pubnub *p)
{
    p->socket = TCPOpen((PTR_BASE) p->origin, TCP_OPEN_RAM_HOST, 80, TCP_PURPOSE_PUBNUB_CLIENT);
    if (p->socket == INVALID_SOCKET)
        return false;
    p->state = PS_CONNECT;
    p->timer = TickGet();
    p->http_substate = 0;
    p->http_code = 0;
    p->http_buf_len = 0;
    return true;
}

static void
pubnub_callback_and_idle(struct pubnub *p, enum pubnub_res result)
{
    /* Reset the context *before* the callback, as the callback may
     * typically want to issue another API call. */
    /* N.B. We ignore p->channel and p->timetoken; it's internal_cb's
     * job to handle these if necessary (shouldn't be necessary). */

    p->state = PS_IDLE;
    p->http_substate = 0;
    int http_code = p->http_code;
    p->http_code = 0;

    if (p->socket != INVALID_SOCKET) {
        TCPDisconnect(p->socket);
        TCPDisconnect(p->socket); // we need to reuse the socket, RST!
        p->socket = INVALID_SOCKET;
    }

#if PUBNUB_SSL
    if (p->ssl) {
        CyaSSL_shutdown(p->ssl);
        CyaSSL_free(p->ssl);
        p->ssl = NULL;
    }
#endif

    char *http_reply = p->http_reply;
    p->http_reply = NULL;
    void *cb = p->cb, *cbdata = p->cbdata;
    p->cb = p->cbdata = NULL;

    if (http_code > 0 && http_code / 100 != 2) {
        /* We received an error HTTP status code; force PNR_HTTP_ERROR
         * result. This can override PNR_OK or a different error
         * signaled later during parsing of the request. */
        result = PNR_HTTP_ERROR;
    }

    p->internal_cb(p, result, http_code, http_reply, cb, cbdata);
}

static bool
pubnub_update_test_timeout(struct pubnub *p)
{
    if (TickGet() - p->timer > p->com_timeout * TICK_SECOND) {
        pubnub_callback_and_idle(p, PNR_TIMEOUT);
        return true;
    }
    return false;
}

#if PUBNUB_SSL
static bool
pubnub_update_sslconnect(struct pubnub *p)
{
    if (!p->ssl) {
        p->ssl = CyaSSL_new(get_ssl_ctx());
        CyaSSL_SetIOReadCtx(p->ssl, &p->socket);
        CyaSSL_SetIOWriteCtx(p->ssl, &p->socket);
    }

    if (CyaSSL_connect(p->ssl) != SSL_SUCCESS) {
        int error = CyaSSL_get_error(p->ssl, 0);
        if (error == SSL_ERROR_WANT_READ ||
                error == SSL_ERROR_WANT_WRITE) {
            /* Operation still in progress. */
            pubnub_update_test_timeout(p);
            return false;

        } else {
            pubnub_callback_and_idle(p, PNR_SSL_ERROR);
            return false;
        }
    }

    /* Connected. */
    return true;
}
#endif

static bool
pubnub_update_sendrequest(struct pubnub *p)
{
    if (!TCPIsConnected(p->socket)) {
        pubnub_callback_and_idle(p, PNR_IO_ERROR);
        return false;
    } else if (pubnub_update_test_timeout(p))
        return false;

    if (p->http_substate <= 0) {
#define S "GET "
        if (!pubnub_tcp_writestr(p, S, sizeof(S)-1))
            return true;
#undef S
        p->http_substate++;
    }
    if (p->http_substate <= 1) {
        int url_len = strlen(p->http_buf.url);
        if (!pubnub_tcp_writestr(p, p->http_buf.url, url_len))
            return true;
        p->http_substate++;
    }
    if (p->http_substate <= 2) {
#define S " HTTP/1.1\r\nHost: "
        if (!pubnub_tcp_writestr(p, S, sizeof(S)-1))
            return true;
#undef S
        p->http_substate++;
    }
    if (p->http_substate <= 3) {
        int namelen = strlen(p->origin);
        if (!pubnub_tcp_writestr(p, p->origin, namelen))
            return true;
        p->http_substate++;
    }
    if (p->http_substate <= 4) {
#define S "\r\nUser-Agent: PubNub-PIC32/0.1\r\nConnection: close\r\n\r\n"
        if (!pubnub_tcp_writestr(p, S, sizeof(S)-1))
            return true;
#undef S
        p->state = PS_HTTPREPLY;
        p->http_substate = 0;
    }
    return true;
}

static void
pubnub_update_recvreply(struct pubnub *p)
{
    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
            return;
        }
        pubnub_update_test_timeout(p);
        return;
    }

    /* http_substate:
     * 0 first line (status code etc.)
     * 1 plain header
     * 2 plain header, chunked encoding detected
     * 3 chunk size line
     * 4 body follows now (transient) */
    while (p->state == PS_HTTPREPLY && readylen > 0) {
        if (p->http_buf_len == sizeof (p->http_buf.line)) {
            /* Our buffer is already full and we did not extract a line.
             * Normally, hitting a long line in the HTTP header should
             * not happen, let's just flush the buffer. */
            p->http_buf_len = 0;
        }
        int gotlen = pubnub_tcp_read(p,
                (BYTE *) p->http_buf.line, p->http_buf_len,
                sizeof(p->http_buf.line)-1);
        p->http_buf_len += gotlen;
        readylen -= gotlen;

        p->http_buf.line[p->http_buf_len] = 0;
        char *bufptr = p->http_buf.line;
        char *newline;
        while (p->state == PS_HTTPREPLY
               && (newline = strstr(bufptr, "\r\n"))) {
            *newline = 0;

            if (newline - bufptr == 0) {
                /* Empty line handling. */
                switch (p->http_substate) {
                    case 1: /* Content follows. */
                        p->http_substate = 4;
                        break;
                    case 2: /* Chunked encoding, another line
                             * with content length follows. */
                        p->http_substate = 3;
                        break;
                    case 0:
                    case 3: /* Error. */
io_error:
                        pubnub_callback_and_idle(p, PNR_IO_ERROR);
                        return; /* straight out */
                }

            } else if (p->http_substate == 0) {
                /* An HTTP status line. */
                if (strncmp(bufptr, "HTTP/1.", 7) || !bufptr[7] || !bufptr[8])
                    goto io_error;
                p->http_code = atoi(bufptr+9);
                /* We do not panic in case of non-200 code; instead,
                 * calmly load the response and only then signal
                 * an error from within pubnub_callback_and_idle(). */
                p->http_substate = 1;

            } else if (p->http_substate < 3) {
                /* An HTTP header line. */
                char h_chunked[] = "Transfer-Encoding: chunked";
                char h_length[] = "Content-Length: ";
                if (!strncmp(bufptr, h_chunked, sizeof(h_chunked)-1)) {
                    p->http_substate = 2;

                } else if (!strncmp(bufptr, h_length, sizeof(h_length)-1)) {
                    p->http_content_length = atoi(bufptr + sizeof(h_length)-1);
                }

            } else if (p->http_substate == 3) {
                /* A chunk size line. */
                p->http_content_length = strtoul(bufptr, NULL, 16);
                p->http_substate = 4;
            }

            if (p->http_substate == 4) {
                p->state = PS_HTTPBODY;
                p->http_reply = NULL;
            }

            p->http_buf_len -= (newline+2 - bufptr);
            bufptr = newline+2;
        }
        memmove(p->http_buf.line, bufptr, p->http_buf_len + 1);
    }
}

static void
pubnub_update_recvbody(struct pubnub *p)
{
    if (!p->http_reply) {
        if (p->http_content_length > PUBNUB_REPLY_MAXLEN) {
            /* Too large reply. Abort and report format error. */
            pubnub_callback_and_idle(p, PNR_FORMAT_ERROR);
            return;
        }
        p->http_reply = malloc(p->http_content_length+1);
        if (p->http_buf_len > p->http_content_length)
            goto io_error;
        memcpy(p->http_reply, p->http_buf.line, p->http_buf_len);
        p->http_content_length -= p->http_buf_len;
    }
    if (p->http_content_length == 0) {
        p->http_reply[p->http_buf_len] = 0;
        p->state = PS_PROCESS;
	return;
    }

    int readylen = TCPIsGetReady(p->socket);
    if (!readylen) {
        if (!TCPIsConnected(p->socket)) {
io_error:
            pubnub_callback_and_idle(p, PNR_IO_ERROR);
            return;
        }
        pubnub_update_test_timeout(p);
        if (p->state == PS_IDLE) {
            free(p->http_reply); p->http_reply = NULL;
        }
        return;
    }

    while (readylen > 0) {
        if (readylen > p->http_content_length)
            goto io_error;

        int gotlen = pubnub_tcp_read(p,
                (BYTE *) p->http_reply, p->http_buf_len,
                p->http_content_length);
        p->http_buf_len += gotlen;
        p->http_content_length -= gotlen;
        readylen -= gotlen;
    }

    if (p->http_content_length == 0) {
        p->http_reply[p->http_buf_len] = 0;
        p->state = PS_PROCESS;
    }
}

void
pubnub_update(struct pubnub *p)
{
    switch (p->state) {
        case PS_IDLE:
            break;

        case PS_CONNECT:
            if (!TCPIsConnected(p->socket)) {
                pubnub_update_test_timeout(p);
                break;
            }
#if !PUBNUB_SSL
            p->state = PS_HTTPREQUEST;
            /* fall-through to PS_HTTPREQUEST */

#else
            if (!p->use_ssl) {
                p->state = PS_HTTPREQUEST;
                break;
            }
            p->state = PS_SSLCONNECT;
            /* fall-through */

        case PS_SSLCONNECT:
            if (pubnub_update_sslconnect(p))
                p->state = PS_HTTPREQUEST;
            break;
#endif

        case PS_HTTPREQUEST:
            if (pubnub_update_sendrequest(p))
                TCPFlush(p->socket);
            break;

        case PS_HTTPREPLY:
            pubnub_update_recvreply(p);
            if (p->state != PS_HTTPBODY)
                break;
            /* ...otherwise fall-through. */

        case PS_HTTPBODY:
            pubnub_update_recvbody(p);
            break;

        case PS_PROCESS:
            pubnub_callback_and_idle(p, PNR_OK);
            break;
    }
}


void
pubnub_cancel(struct pubnub *p)
{
    pubnub_callback_and_idle(p, PNR_CANCELLED);
}


void
pubnub_publish_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (cb)
        ((pubnub_publish_cb) cb)(p, result, http_code, reply, cbdata);
    free(reply);
}

bool
pubnub_publish(struct pubnub *p, const char *channel, const char *message,
        pubnub_publish_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    if (p->http_reply)
        free(p->http_reply);
    p->http_reply = NULL;
    p->http_content_length = 0;

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/publish/%s/%s/0/%s/0/", p->publish_key, p->subscribe_key,
            channel);

    const char *pmessage = message;
    while (pmessage[0]) {
        /* RFC 3986 Unreserved characters plus few
         * safe reserved ones. */
        size_t okspan = strspn(pmessage, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~" ",=:;@[]");
        if (okspan > 0) {
            if (okspan > sizeof(p->http_buf.url)-1 - p->http_buf_len) {
                p->http_buf_len = 0;
                return false;
            }
            memcpy(&p->http_buf.url[p->http_buf_len], pmessage, okspan);
            p->http_buf_len += okspan;
            p->http_buf.url[p->http_buf_len] = 0;
            pmessage += okspan;
        }
        if (pmessage[0]) {
            /* %-encode a non-ok character. */
            char enc[4] = {'%'};
            enc[1] = "0123456789ABCDEF"[pmessage[0] / 16];
            enc[2] = "0123456789ABCDEF"[pmessage[0] % 16];
            if (3 > sizeof(p->http_buf.url)-1 - p->http_buf_len) {
                p->http_buf_len = 0;
                return false;
            }
            memcpy(&p->http_buf.url[p->http_buf_len], enc, 4);
            p->http_buf_len += 3;
            pmessage++;
        }
    }

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_publish_icb;
    p->channel = channel;
    p->com_timeout = p->timeout;
    return pubnub_http_connect(p);
}


void
pubnub_subscribe_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (result != PNR_OK) {
error:
#ifdef PUBNUB_MISSMSG_OK
	if (1) {
#else
        if (result == PNR_FORMAT_ERROR) {
#endif
            /* In case of PubNub protocol error, abort an ongoing subscribe
             * and start over. This means some messages were lost, but allows
             * us to recover from bad situations, e.g. too many messages
             * queued or unexpected problem caused by a particular message. */
            strcpy(p->timetoken, "0");
        }
        if (cb)
            ((pubnub_subscribe_cb) cb)(p, result, http_code, p->channel, reply, cbdata);
        return;
    }
    int replylen = strlen(reply);
    if (reply[replylen-1] != ']' && replylen > 2)
        replylen -= 2; // XXX: this seems required by Manxiang
    if (reply[0] != '[' || reply[replylen-1] != ']'
        || reply[replylen-2] != '"') {
        result = PNR_FORMAT_ERROR;
        goto error;
    }

    /* Extract the last argument. */
    int i = extract_last_string(reply, replylen-2);
    if (i < 0) {
        result = PNR_FORMAT_ERROR;
        goto error;
    }

    /* Now, the last argument may either be a timetoken or a channel list. */
    if (reply[i-2] == '"') {
        /* It is a channel list, there is another string argument in front
         * of us. Process the channel list ... */
        p->chan_ofs = i+1;
        p->chan_end = replylen;
        int k;
        for (k = p->chan_end-1; k > p->chan_ofs; k--)
            if (reply[k] == ',')
                reply[k] = 0;

        /* ... and look for timetoken again. */
        i = extract_last_string(reply, i-2);
        if (i < 0) {
            result = PNR_FORMAT_ERROR;
            goto error;
        }

    } else {
        p->chan_ofs = 0;
        p->chan_end = 0;
    }

    /* Now, i points at
     * [[1,2,3],"5678"]
     * [[1,2,3],"5678","a,b,c"]
     *          ^-- here */

    /* Setup timetoken. */
    if (replylen-2 - (i+1) >= 64) {
        result = PNR_FORMAT_ERROR;
        goto error;
    }
    strcpy(p->timetoken, &reply[i+1]);
    reply[i-2] = 0; // terminate the [] message array (before the ]!)

    /* As a special case, restart the subscribe if we are dealing
     * with an empty reply. */
    if (i-2 == 2) {
        free(reply);
        if (!pubnub_subscribe(p, p->channel, cb, cbdata) && cb)
            ((pubnub_subscribe_cb) cb)(p, PNR_IO_ERROR, http_code, p->channel, NULL, cbdata);
        return;
    }

    p->http_reply = reply;

    /* Set up the message list - offset, length and NUL-characters splitting
     * the messages. */
    p->msg_ofs = 2;
    p->msg_end = i-2;
    if (!split_array(&p->http_reply[p->msg_ofs], p->msg_end - p->msg_ofs)) {
        p->http_reply = NULL;
        if (cb)
            ((pubnub_subscribe_cb) cb)(p, PNR_FORMAT_ERROR, http_code, p->channel, reply, cbdata);
        free(reply);
    }

    /* Return the first message. */
    if (cb)
        ((pubnub_subscribe_cb) cb)(p, PNR_OK, http_code,
            p->chan_ofs ? &p->http_reply[p->chan_ofs] : p->channel,
            &p->http_reply[p->msg_ofs], cbdata);
}

bool
pubnub_subscribe(struct pubnub *p, const char *channel,
        pubnub_subscribe_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;

    if (p->http_reply) {
        int prevlen = strlen(&p->http_reply[p->msg_ofs]) + 1;
        p->msg_ofs += prevlen;
        if (p->msg_ofs < p->msg_end) {
            /* Next message from stash-away buffer. */

            char *msgchan = "";
            int prevchlen = strlen(&p->http_reply[p->chan_ofs]) + 1;
            if (p->chan_ofs > 0 && p->chan_ofs < p->chan_end) {
                p->chan_ofs += prevchlen;
                if (p->chan_ofs < p->chan_end)
                    msgchan = &p->http_reply[p->chan_ofs];
            }

            if (cb)
                cb(p, PNR_OK, 0,
                    p->chan_ofs ? msgchan : p->channel,
                    &p->http_reply[p->msg_ofs], cb_data);
            return true;
        }
        /* That's all. free() below and fetch new messages. */
    }

    if (p->http_reply)
        free(p->http_reply);
    p->http_reply = NULL;
    p->http_buf_len = 0;

    p->http_content_length = 0;

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/subscribe/%s/%s/0/%s?" "%s%s" "%s%s%s", p->subscribe_key,
            channel, p->timetoken,
            p->uuid ? "uuid=" : "", p->uuid ? p->uuid : "",
            p->uuid && p->auth ? "&" : "",
            p->auth ? "auth=" : "", p->auth ? p->auth : "");

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_subscribe_icb;
    p->channel = channel;
    p->com_timeout = p->sub_timeout;
    return pubnub_http_connect(p);
}


void
pubnub_leave_icb(struct pubnub *p, enum pubnub_res result, int http_code,
        char *reply, void *cb, void *cbdata)
{
    if (cb)
        ((pubnub_leave_cb) cb)(p, result, http_code, reply, cbdata);
    free(reply);
}

bool
pubnub_leave(struct pubnub *p, const char *channel,
        pubnub_leave_cb cb, void *cb_data)
{
    if (p->state != PS_IDLE)
        return false;
    if (p->http_reply)
        free(p->http_reply);
    p->http_reply = NULL;
    p->http_content_length = 0;

    /* Make sure next subscribe() will be a join. */
    strcpy(p->timetoken, "0");

    p->http_buf_len = snprintf(p->http_buf.url, sizeof(p->http_buf.url),
            "/v2/presence/sub-key/%s/channel/%s/leave?" "%s%s" "%s%s%s",
            p->subscribe_key, channel,
            p->uuid ? "uuid=" : "", p->uuid ? p->uuid : "",
            p->uuid && p->auth ? "&" : "",
            p->auth ? "auth=" : "", p->auth ? p->auth : "");

    p->cb = cb; p->cbdata = cb_data;
    p->internal_cb = pubnub_leave_icb;
    p->channel = channel;
    p->com_timeout = p->timeout;
    return pubnub_http_connect(p);
}
