
export default class BaseNode extends cc.Node{

    private myChangedDirtyFlag:boolean = null;
    private myDeleteDirtyFlag: boolean = null;

    constructor(){
        super("BaseNode");

        // this.setName("BaseNode");

        this.myChangedDirtyFlag = false;
        this.myDeleteDirtyFlag = false;
    }

    public setMyChangedDirtyFlag(dirtyFlag: boolean){
        this.myChangedDirtyFlag = dirtyFlag;
    }

    public setMyDeleteDirtyFlag(dirtyFlag: boolean){
        this.myDeleteDirtyFlag = dirtyFlag;
    }

    public doEnter(){

    }

    public doExit(){

    }
};