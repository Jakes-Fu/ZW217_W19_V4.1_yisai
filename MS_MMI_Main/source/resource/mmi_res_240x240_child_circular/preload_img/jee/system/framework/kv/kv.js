
console.log("kv.js loading");
export class KV{
     getKeyValue(key){
		return js_kv_get_value(key);
	}

	setKeyValue(key,value){
		return js_kv_set_value(key,value);
	}

	removeKeyValue(key){
		return js_kv_delete_value(key);
	}
}

console.log("kv.js ok");

