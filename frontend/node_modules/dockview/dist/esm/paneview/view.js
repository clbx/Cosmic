import { ReactPart } from '../react';
export class PanePanelSection {
    get element() {
        return this._element;
    }
    constructor(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._element = document.createElement('div');
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    init(parameters) {
        this.part = new ReactPart(this.element, this.reactPortalStore, this.component, {
            params: parameters.params,
            api: parameters.api,
            title: parameters.title,
            containerApi: parameters.containerApi,
        });
    }
    toJSON() {
        return {
            id: this.id,
        };
    }
    update(params) {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update(params.params);
    }
    dispose() {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    }
}
