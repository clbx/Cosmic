import { ReactPart } from '../react';
export class ReactWatermarkPart {
    get element() {
        return this._element;
    }
    constructor(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    init(parameters) {
        this.part = new ReactPart(this.element, this.reactPortalStore, this.component, {
            group: parameters.group,
            containerApi: parameters.containerApi,
        });
    }
    focus() {
        // noop
    }
    update(params) {
        var _a, _b, _c;
        if (this.parameters) {
            this.parameters.params = params.params;
        }
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update({ params: (_c = (_b = this.parameters) === null || _b === void 0 ? void 0 : _b.params) !== null && _c !== void 0 ? _c : {} });
    }
    layout(_width, _height) {
        // noop - retrieval from api
    }
    dispose() {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    }
}
