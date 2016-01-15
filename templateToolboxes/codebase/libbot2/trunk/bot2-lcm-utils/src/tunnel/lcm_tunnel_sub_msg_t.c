/** THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
 * BY HAND!!
 *
 * Generated by lcm-gen
 **/

#include <string.h>
#include "lcm_tunnel_sub_msg_t.h"

static int __lcm_tunnel_sub_msg_t_hash_computed;
static int64_t __lcm_tunnel_sub_msg_t_hash;
 
int64_t __lcm_tunnel_sub_msg_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __lcm_tunnel_sub_msg_t_get_hash)
            return 0;
 
    const __lcm_hash_ptr cp = { p, (void*)__lcm_tunnel_sub_msg_t_get_hash };
    (void) cp;
 
    int64_t hash = 0xbd03d96a31b72228LL
         + __string_hash_recursive(&cp)
         + __int32_t_hash_recursive(&cp)
         + __byte_hash_recursive(&cp)
        ;
 
    return (hash<<1) + ((hash>>63)&1);
}
 
int64_t __lcm_tunnel_sub_msg_t_get_hash(void)
{
    if (!__lcm_tunnel_sub_msg_t_hash_computed) {
        __lcm_tunnel_sub_msg_t_hash = __lcm_tunnel_sub_msg_t_hash_recursive(NULL);
        __lcm_tunnel_sub_msg_t_hash_computed = 1;
    }
 
    return __lcm_tunnel_sub_msg_t_hash;
}
 
int __lcm_tunnel_sub_msg_t_encode_array(void *buf, int offset, int maxlen, const lcm_tunnel_sub_msg_t *p, int elements)
{
    int pos = 0, thislen, element;
 
    for (element = 0; element < elements; element++) {
 
        thislen = __string_encode_array(buf, offset + pos, maxlen - pos, &(p[element].channel), 1);
        if (thislen < 0) return thislen; else pos += thislen;
 
        thislen = __int32_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].data_size), 1);
        if (thislen < 0) return thislen; else pos += thislen;
 
        thislen = __byte_encode_array(buf, offset + pos, maxlen - pos, p[element].data, p[element].data_size);
        if (thislen < 0) return thislen; else pos += thislen;
 
    }
    return pos;
}
 
int lcm_tunnel_sub_msg_t_encode(void *buf, int offset, int maxlen, const lcm_tunnel_sub_msg_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __lcm_tunnel_sub_msg_t_get_hash();
 
    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
 
    thislen = __lcm_tunnel_sub_msg_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;
 
    return pos;
}
 
int __lcm_tunnel_sub_msg_t_encoded_array_size(const lcm_tunnel_sub_msg_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {
 
        size += __string_encoded_array_size(&(p[element].channel), 1);
 
        size += __int32_t_encoded_array_size(&(p[element].data_size), 1);
 
        size += __byte_encoded_array_size(p[element].data, p[element].data_size);
 
    }
    return size;
}
 
int lcm_tunnel_sub_msg_t_encoded_size(const lcm_tunnel_sub_msg_t *p)
{
    return 8 + __lcm_tunnel_sub_msg_t_encoded_array_size(p, 1);
}
 
int __lcm_tunnel_sub_msg_t_decode_array(const void *buf, int offset, int maxlen, lcm_tunnel_sub_msg_t *p, int elements)
{
    int pos = 0, thislen, element;
 
    for (element = 0; element < elements; element++) {
 
        thislen = __string_decode_array(buf, offset + pos, maxlen - pos, &(p[element].channel), 1);
        if (thislen < 0) return thislen; else pos += thislen;
 
        thislen = __int32_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].data_size), 1);
        if (thislen < 0) return thislen; else pos += thislen;
 
        p[element].data = (uint8_t*) lcm_malloc(sizeof(uint8_t) * p[element].data_size);
        thislen = __byte_decode_array(buf, offset + pos, maxlen - pos, p[element].data, p[element].data_size);
        if (thislen < 0) return thislen; else pos += thislen;
 
    }
    return pos;
}
 
int __lcm_tunnel_sub_msg_t_decode_array_cleanup(lcm_tunnel_sub_msg_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {
 
        __string_decode_array_cleanup(&(p[element].channel), 1);
 
        __int32_t_decode_array_cleanup(&(p[element].data_size), 1);
 
        __byte_decode_array_cleanup(p[element].data, p[element].data_size);
        if (p[element].data) free(p[element].data);
 
    }
    return 0;
}
 
int lcm_tunnel_sub_msg_t_decode(const void *buf, int offset, int maxlen, lcm_tunnel_sub_msg_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __lcm_tunnel_sub_msg_t_get_hash();
 
    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;
 
    thislen = __lcm_tunnel_sub_msg_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;
 
    return pos;
}
 
int lcm_tunnel_sub_msg_t_decode_cleanup(lcm_tunnel_sub_msg_t *p)
{
    return __lcm_tunnel_sub_msg_t_decode_array_cleanup(p, 1);
}
 
int __lcm_tunnel_sub_msg_t_clone_array(const lcm_tunnel_sub_msg_t *p, lcm_tunnel_sub_msg_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {
 
        __string_clone_array(&(p[element].channel), &(q[element].channel), 1);
 
        __int32_t_clone_array(&(p[element].data_size), &(q[element].data_size), 1);
 
        q[element].data = (uint8_t*) lcm_malloc(sizeof(uint8_t) * q[element].data_size);
        __byte_clone_array(p[element].data, q[element].data, p[element].data_size);
 
    }
    return 0;
}
 
lcm_tunnel_sub_msg_t *lcm_tunnel_sub_msg_t_copy(const lcm_tunnel_sub_msg_t *p)
{
    lcm_tunnel_sub_msg_t *q = (lcm_tunnel_sub_msg_t*) malloc(sizeof(lcm_tunnel_sub_msg_t));
    __lcm_tunnel_sub_msg_t_clone_array(p, q, 1);
    return q;
}
 
void lcm_tunnel_sub_msg_t_destroy(lcm_tunnel_sub_msg_t *p)
{
    __lcm_tunnel_sub_msg_t_decode_array_cleanup(p, 1);
    free(p);
}
 
int lcm_tunnel_sub_msg_t_publish(lcm_t *lc, const char *channel, const lcm_tunnel_sub_msg_t *p)
{
      int max_data_size = lcm_tunnel_sub_msg_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = lcm_tunnel_sub_msg_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _lcm_tunnel_sub_msg_t_subscription_t {
    lcm_tunnel_sub_msg_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void lcm_tunnel_sub_msg_t_handler_stub (const lcm_recv_buf_t *rbuf, 
                            const char *channel, void *userdata)
{
    int status;
    lcm_tunnel_sub_msg_t p;
    memset(&p, 0, sizeof(lcm_tunnel_sub_msg_t));
    status = lcm_tunnel_sub_msg_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding lcm_tunnel_sub_msg_t!!!\n", status);
        return;
    }

    lcm_tunnel_sub_msg_t_subscription_t *h = (lcm_tunnel_sub_msg_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    lcm_tunnel_sub_msg_t_decode_cleanup (&p);
}

lcm_tunnel_sub_msg_t_subscription_t* lcm_tunnel_sub_msg_t_subscribe (lcm_t *lcm, 
                    const char *channel, 
                    lcm_tunnel_sub_msg_t_handler_t f, void *userdata)
{
    lcm_tunnel_sub_msg_t_subscription_t *n = (lcm_tunnel_sub_msg_t_subscription_t*)
                       malloc(sizeof(lcm_tunnel_sub_msg_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel, 
                                 lcm_tunnel_sub_msg_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg lcm_tunnel_sub_msg_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int lcm_tunnel_sub_msg_t_unsubscribe(lcm_t *lcm, lcm_tunnel_sub_msg_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr, 
           "couldn't unsubscribe lcm_tunnel_sub_msg_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}
