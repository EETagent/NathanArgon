#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <microhttpd.h>
#include <argon2.h>
#include <parson.h>

// Port webové služby
#define PORT 80

struct postStatus
{
    bool status;
    char *buff;
};

static int hash_api(void *cls,
                    struct MHD_Connection *connection,
                    const char *url,
                    const char *method,
                    const char *version,
                    const char *upload_data,
                    size_t *upload_data_size,
                    void **ptr)
{
    // Pouze pro endpoint hash nebo api a metodu POST
    if (strcmp(url, "/hash") == 0 || strcmp(url, "/api") == 0 && strcmp(method, "POST") == 0)
    {
        const char *page = cls;
        struct MHD_Response *response;
        int ret;

        struct postStatus *post = NULL;
        post = (struct postStatus *)*ptr;

        if (post == NULL)
        {
            post = malloc(sizeof(struct postStatus));
            post->status = false;
            *ptr = post;
        }
        if (!post->status)
        {
            post->status = true;
            return MHD_YES;
        }
        else
        {
            if (*upload_data_size != 0)
            {
                // Zpracování požadavku
                post->buff = malloc(*upload_data_size + 1);
                snprintf(post->buff, *upload_data_size + 1, "%s", upload_data);
                *upload_data_size = 0;
                return MHD_YES;
            }
            else
            {
                // Deserializace těla POST požadavku
                JSON_Value *request_schema = json_parse_string(post->buff);
                JSON_Object *request_object = json_value_get_object(request_schema);

                // Serializace odpovědi serveru
                JSON_Value *response_schema = json_value_init_object();
                JSON_Object *response_object = json_value_get_object(response_schema);
                // Dirty hack: Zatím s plain, aby jsme ověřili request a nemuseli JSONy vytvářet tři, ten se pak přepíše na klíč hash
                json_object_set_string(response_object, "plain", "");

                // Ověření validity požadavku
                int validate = json_validate(request_schema, response_schema);

                // Pokud je request ve správném formátu
                if (validate == 0)
                {
                    uint8_t *pwd = (uint8_t *)json_object_dotget_string(request_object, "plain");
                    uint32_t pwdlen = strlen((char *)pwd);
                    uint8_t salt[16];
                    // Generování saltu
                    memset(salt, 0x00, 16);
                    char encoded[100];
                    // Hashování pomocí Argon2 ID
                    // Hodnoty zadané: 100 smyček | 4096 KiB paměti | 1 vlákno
                    argon2id_hash_encoded(100, 4096, 1, pwd, pwdlen, salt, 16, 32, encoded, 100);
                    // Vložení do serializované odpovědi serveru, nahrazení plain hashem
                    json_object_remove(response_object, "plain");
                    json_object_set_string(response_object, "hash", encoded);
                    // Odpověď serveru
                    page = json_serialize_to_string(response_schema);
                }
                // V případě chybného request
                else
                {
                    // Chybová odpověď serveru
                    page = "Chybný požadavek";
                }
                // Vyčištění paměti
                free(post->buff);
                json_value_free(request_schema);
                json_value_free(response_schema);
            }
        }

        // Vyčištění paměti
        free(post);

        // Odeslání odpovědi
        response = MHD_create_response_from_buffer(strlen(page),
                                                   (void *)page,
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection,
                                 MHD_HTTP_OK,
                                 response);
        MHD_destroy_response(response);

        return ret;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    int port;
    char* env_port = getenv("NATHAN_ARGON_PORT");

    if(env_port)
    {
        port = atoi(env_port);
        if (port == 0) {
            port = PORT;
        }
    }
    else
    {
        port = PORT;
    }
    // Vytvoření webového serveru
    struct MHD_Daemon *d;
    d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                         port,
                         NULL,
                         NULL,
                         (MHD_AccessHandlerCallback)&hash_api,
                         NULL,
                         MHD_OPTION_END);

    // Selže-li vytvoření webového serveru
    if (d == NULL)
    {
        fprintf(stderr, "Selhalo vytvoření webového serveru, port může být obsazený nebo vyžadovat zvýšená oprávnění");
        return 1;
    }

    // Ukončení programu enterem/znakem
    getchar();

    MHD_stop_daemon(d);

    return 0;
}
