#include "connection.h"

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / ((double) RAND_MAX + 1.0);

    return (int)(rand() * fraction * (max - min + 1) + min);
}

void RRCConnectionRequestCoder(uint8_t **buffer, ssize_t *len)
{
    RRCConnectionRequest_t rrc_connection_request;
    int ue_id = random();

    // Устанавливаем запрос на подключение RRC
    memset(&rrc_connection_request, 0, sizeof(rrc_connection_request));
    rrc_connection_request.criticalExtensions.present;
    rrc_connection_request.criticalExtensions.choice.rrcConnectionRequest_r8.establishmentCause = EstablishmentCause_mt_Access;
    rrc_connection_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.present = InitialUE_Identity_PR_randomValue;
    rrc_connection_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.buf = (uint8_t*)&ue_id;
    rrc_connection_request.criticalExtensions.choice.rrcConnectionRequest_r8.ue_Identity.choice.randomValue.size = sizeof(ue_id);

    // xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, &rrc_connection_request);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionRequest, &rrc_connection_request);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL)
    {
        fprintf(stderr, "Error when receiving an RRC connection request\n");
        exit(1);
    } else
    {
        fprintf(stderr, "Encoded RRC connection request\n");
    }

}

bool RRCConnectionRequestDecoder(RRCConnectionRequest_t *rrc_request, uint8_t *buffer, size_t len)
{
    asn_dec_rval_t rval;
   
    rval = asn_decode(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionRequest, (void **)&rrc_request, (void *)buffer, len);
    
    if (rval.code != RC_OK) 
    {
        // если ошибка 
        return false;
    }

    // xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, rrc_request);

    return true;
}

void RRCConnectionCompleteCoder(uint8_t **buffer, ssize_t *len)
{
    RRCConnectionSetup_t rrc_connection_setup;
    long srb_Identity = getRandomNumber(1, 2);
    long rrc_TransactionIdentifier = getRandomNumber(0, 3);
    long eps_BearerIdentity = getRandomNumber(0, 15);


    // set RRC connection setup
    memset(&rrc_connection_setup, 0, sizeof(rrc_connection_setup));
    rrc_connection_setup.radioResourceConfigDedicated.eps_BearerIdentity = &eps_BearerIdentity;
    rrc_connection_setup.radioResourceConfigDedicated.srb_Identity = srb_Identity;
    rrc_connection_setup.rrc_TransactionIdentifier = rrc_TransactionIdentifier;

    // xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, &rrc_connection_setup);

    asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
    res = asn_encode_to_new_buffer(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionSetup, &rrc_connection_setup);
    *buffer = res.buffer;
    *len = res.result.encoded;

    if (*buffer == NULL)
    {
        fprintf(stderr, "Error when receiving an RRC connection request\n");
        exit(1);
    } else
    {
        fprintf(stderr, "RRC Connect Setup\n");
    }
}

bool RRCConnectionCompleteDecoder(RRCConnectionSetup_t *rrc_connection_setup, uint8_t *buffer, size_t len)
{
    asn_dec_rval_t rval;
   
    rval = asn_decode(NULL, ATS_UNALIGNED_BASIC_PER, &asn_DEF_RRCConnectionSetup, (void **)&rrc_connection_setup, (void *)buffer, len);
    
    if (rval.code != RC_OK) 
    {
        return false;
    }

    // xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, rrc_connection_setup);

    return true;
}