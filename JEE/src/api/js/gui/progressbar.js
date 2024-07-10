//define progressbar
import {Component} from 'component';
export class Progressbar extends Component{
    constructor(handle, id){

        if (arguments.length == 2) {
            super.create(handle, id);
            this._nativeHandle = js_gui_progressbar_create(this, this._parentHandle);
        } else {
            super();
        }

       

        console.log("Progressbar: constructor");
        this.TAG = "Progressbar:";
        this._step = "";
        this._is_slide = "";
        this._type = "";
        this._prg_bg_color = "";
        this._prg_fg_color = "";
        this._percent = "";

        Object.defineProperty(this, "step",{
            enumerable:true,
            get:function(){
                return this._step;
            },
            set:function(newValue){
               {
                    this._step  =any2number(newValue);;
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_step(this._nativeHandle, this._step);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "is_slide",{
            enumerable:true,
            get:function(){
                return this._is_slide;
            },
            set:function(newValue){
                var bValue=true;
                if(typeof newValue !== "boolean"){
                    console.log(this.TAG+"is_slide type error");
                    //throw "type error";
                    bValue= (newValue==='true')
                }else{
                    bValue=newValue;       

                }

                {
                    this._is_slide = bValue;
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_slide(this._nativeHandle,  this._is_slide);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "type",{
            enumerable:true,
            get:function(){
                return this._type;
            },
            set:function(newValue){
                if(typeof newValue !== "string"){
                    console.log(this.TAG+"type error");
                    //throw "type error";
                }
                else{
                    this._type = newValue;
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_type(this._nativeHandle, newValue);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "prg_bg_color",{
            enumerable:true,
            get:function(){
                return this._prg_bg_color;
            },
            set:function(newValue){
              {
                    this._prg_bg_color = any2number(newValue);
                    
                    console.log()
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_bg_color(this._nativeHandle, this._prg_bg_color);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "prg_fg_color",{
            enumerable:true,
            get:function(){
                return this._prg_fg_color;
            },
            set:function(newValue){
               {
                    this._prg_fg_color = any2number(newValue);
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_fg_color(this._nativeHandle, this._prg_fg_color );
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })

        Object.defineProperty(this, "percent",{
            enumerable:true,
            get:function(){
                return this._percent;
            },
            set:function(newValue){
                if(typeof newValue !== "number"){
                    console.log(this.TAG+"percent type error");
                    //throw "type error";
                }
                else if(newValue <= 100){
                    this._percent = newValue;
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_percent(this._nativeHandle, newValue);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
                else{
                    newValue = 100;
                    this._percent = newValue;
                    console.log(this.TAG+"the percent has exceeded 100");
                    if(null !== this._nativeHandle){
                        js_gui_progressbar_set_percent(this._nativeHandle, newValue);
                    }
                    else{
                        console.log(this.TAG+"this._nativeHandle === null");
                    }
                }
            }
        })
    }

    create(handle, id) {
        super.create(handle, id);
        console.log("progressbar: create");
        this._nativeHandle = js_gui_progressbar_create(this, this._parentHandle);
        return this._nativeHandle;
    }
    delete(){
        super.delete();

        console.log("Progressbar: delete");
        js_gui_progressbar_destory(this._nativeHandle);
    }
}

