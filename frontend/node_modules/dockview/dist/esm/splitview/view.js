import { SplitviewApi, SplitviewPanel, } from 'dockview-core';
import { ReactPart } from '../react';
export class ReactPanelView extends SplitviewPanel {
    constructor(id, component, reactComponent, reactPortalStore) {
        super(id, component);
        this.reactComponent = reactComponent;
        this.reactPortalStore = reactPortalStore;
    }
    getComponent() {
        var _a, _b;
        return new ReactPart(this.element, this.reactPortalStore, this.reactComponent, {
            params: (_b = (_a = this._params) === null || _a === void 0 ? void 0 : _a.params) !== null && _b !== void 0 ? _b : {},
            api: this.api,
            containerApi: new SplitviewApi(this._params.accessor),
        });
    }
}
