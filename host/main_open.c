/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>
//#include <ta_storage.h>
#include <tee_api_defines.h>
#include <tee_api_defines_extensions.h>
#include <tee_api_types.h>

/* To the the UUID (found the the TA's h-file(s)) */
#include <hello_world_ta.h>

#define DATA_SIZE	256

int main(int argc, char *argv[])
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_HELLO_WORLD_UUID;
	uint32_t err_origin;

    //TEEC_SharedMemory inputSM;

    //Variables for fs_create (CreatePersistentObject)
    static uint8_t id[] = {
            0x01, 0x00
    };
    size_t id_size = sizeof(id);
    char data[DATA_SIZE];
    strcpy(data,"testinput");
    size_t data_size = sizeof(data);
    uint32_t flags_create = TEE_DATA_FLAG_ACCESS_WRITE |
            TEE_DATA_FLAG_ACCESS_READ |
            TEE_DATA_FLAG_ACCESS_WRITE_META;

    uint32_t flags_open = TEE_DATA_FLAG_ACCESS_WRITE_META;




	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open a session to the "hello world" TA, the TA will print "hello
	 * world!" in the log when the session is created.
	 */
	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	/*
	 * Execute a function in the TA by invoking it, in this case
	 * we're incrementing a number.
	 *
	 * The value of command ID part and how the parameters are
	 * interpreted is part of the interface provided by the TA.
	 */

	/* Clear the TEEC_Operation struct */
	memset(&op, 0, sizeof(op));


	/*
	 * Prepare the argument. Pass a value in the first parameter,
	 * the remaining three parameters are unused.
	 */

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE,
					 TEEC_NONE, TEEC_NONE);
    op.params[0].value.a = 45;

	/*
	 * TA_HELLO_WORLD_CMD_INC_VALUE is the actual function   in the TA to be
	 * called.
	 */
	printf("Invoking TA to increment %d\n", op.params[0].value.a);
	res = TEEC_InvokeCommand(&sess, TA_HELLO_WORLD_CMD_INC_VALUE, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
	printf("TA incremented value to %d\n", op.params[0].value.a);


    /* Clear the TEEC_Operation struct */
    memset(&op, 0, sizeof(op));
    //memset(&inputSM, 'b', sizeof(inputSM));
    /*
     * SharedMemory input with key for password
     * Failed attempt to manually send shared memory to TA
     */
    //data = argv[0];
    //strcpy(data,"testinput");
    //inputSM.buffer = data;
    /*op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_NONE,
                                     TEEC_NONE, TEEC_NONE);
    op.params[0].memref.parent = &inputSM;*/

    /*
     * CreatePersistentObject Invoke
     */
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                     TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
                                     TEEC_MEMREF_TEMP_INPUT);
    op.params[0].tmpref.buffer = id;
    op.params[0].tmpref.size = id_size;
    op.params[1].value.a = flags_create;
    op.params[1].value.b = 0;
    op.params[2].value.a = 0;
    op.params[2].value.b = TEE_STORAGE_PRIVATE; // storage_id
    op.params[3].tmpref.buffer = data;
    op.params[3].tmpref.size = data_size;
    /*
     * CREATE_OBJECT TA function to CreatePersistentObject
     */
    printf("Creating TEE persistent object\n");
    res = TEEC_InvokeCommand(&sess, CREATE_OBJECT, &op,
                             &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
             res, err_origin);
    printf("Done, returned object #%d\n",op.params[1].value.b);

    /* Clear the TEEC_Operation struct */
    memset(&op, 0, sizeof(op));

    /*
     * OpenPersistentObject Invoke
     */
    op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
                                     TEEC_VALUE_INOUT, TEEC_VALUE_INPUT,
                                     TEEC_NONE);
    op.params[0].tmpref.buffer = id;
    op.params[0].tmpref.size = id_size;
    op.params[1].value.a = flags_open;
    op.params[1].value.b = 0;
    op.params[2].value.a = TEE_STORAGE_PRIVATE; //storage_id
    /*
     * TEST_WRITE TA function to CreatePersistentObject
     */
    printf("Opening TEE persistent object\n");
    res = TEEC_InvokeCommand(&sess, OPEN_OBJECT, &op,
                             &err_origin);
    if (res != TEEC_SUCCESS)
        errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
             res, err_origin);
    printf("Done, returned object #%d\n",op.params[1].value.b);
	/*
	 * We're done with the TA, close the session and
	 * destroy the context.
	 *
	 * The TA will print "Goodbye!" in the log when the
	 * session is closed.
	 */

	TEEC_CloseSession(&sess);

	TEEC_FinalizeContext(&ctx);

	return 0;
}
