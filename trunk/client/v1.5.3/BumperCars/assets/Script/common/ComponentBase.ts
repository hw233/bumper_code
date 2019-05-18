import EventCenter  from "./EventCenter";
import EngineUtil   from "./EngineUtil";

import {MessageType} from "./EventCenter";

//组件基类
export default class ComponentBase extends cc.Component
{
    /**
     * 子类重写此函数后，必须调用父类的这个函数
     */
    protected onDestroy()
    {
        //组件销毁时 清除所有注册的事件
        EventCenter.RemoveEvent(this.uuid);
    }

    //注册自定义事件
    protected registerEvent(key: string, callback: Function)
    {
        EventCenter.RegisterEvent(this.uuid, key, callback);
    }

    protected removeEvent(key: string)
    {
        EventCenter.RemoveEventByKey(this.uuid, key);
    }

    //派发自定义事件-同步
    protected dispatchEvent(key: string, value = null)
    {
        EventCenter.DispatchEvent(key, value);
    }

    //派发自定义事件-异步
    protected dispatchEventAsync(key: string)
    {
        EventCenter.DispatchEventAsync(key);
    }

    //注册按钮点击事件
    protected registerClickEvent(target: cc.Node, callback: Function)
    {
        if(target && callback)
        {
            EngineUtil.RegBtnClickEvent(target, callback);
        }
    }

    //注册手指离开屏幕事件
    protected registerTouchEndEvent(target: cc.Node, callback: Function)
    {
        if(target && callback)
        {
            EngineUtil.RegTouchEndEvent(target, callback);
        }
    }
}
