/**
 * XMSS Verification Operation
 * (C) 2016 Matthias Gierlings
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 **/

#ifndef BOTAN_XMSS_VERIFICATION_OPERATION_H__
#define BOTAN_XMSS_VERIFICATION_OPERATION_H__

#include <array>
#include <cstddef>
#include <iterator>
#include <string>
#include <botan/assert.h>
#include <botan/types.h>
#include <botan/xmss_publickey.h>
#include <botan/xmss_common_ops.h>
#include <botan/internal/pk_ops.h>
#include <botan/internal/xmss_signature.h>

namespace Botan {

/**
 * Provides signature verification capabilities for Extended Hash-Based
 * Signatures (XMSS).
 **/
 class BOTAN_DLL XMSS_Verification_Operation
   : public virtual PK_Ops::Verification,
     public XMSS_Common_Ops
   {
   public:
      typedef XMSS_PublicKey Key_Type;

      XMSS_Verification_Operation(
         const XMSS_PublicKey& public_key);

      virtual ~XMSS_Verification_Operation() {}

      virtual size_t max_input_bits() const override
         {
         return m_pub_key.max_input_bits();
         }

      virtual size_t message_part_size() const override
         {
         return m_pub_key.message_part_size();
         }

      virtual size_t message_parts() const override
         {
         return m_pub_key.message_parts();
         }

      virtual bool is_valid_signature(const byte sig[],
                                      size_t sig_len) override;

      void update(const byte msg[], size_t msg_len) override;

   private:
      /**
       * Algorithm 13: "XMSS_rootFromSig"
       * Computes a root node using an XMSS signature, a message and a seed.
       *
       * @param msg A message.
       * @param sig The XMSS signature for msg.
       * @param adrs A XMSS tree address.
       * @param seed A seed.
       *
       * @return An n-byte string holding the value of the root of a tree
       *         defined by the input parameters.
       **/
      secure_vector<byte> root_from_signature(
         const XMSS_Signature& sig,
         const secure_vector<byte>& msg,
         XMSS_Address& ards,
         const secure_vector<byte>& seed);

      /**
       * Algorithm 14: "XMSS_verify"
       * Verifies a XMSS signature using the corresponding XMSS public key.
       *
       * @param sig A XMSS signature.
       * @param msg The message signed with sig.
       * @paeam pub_key
       *
       * @return true if signature sig is valid for msg, false otherwise.
       **/
      bool verify(const XMSS_Signature& sig,
                  const secure_vector<byte>& msg,
                  const XMSS_PublicKey& pub_key);

      XMSS_PublicKey m_pub_key;
      secure_vector<byte> m_msg_buf;
   };

}

#endif
