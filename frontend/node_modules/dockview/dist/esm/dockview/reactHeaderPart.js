import { ReactPart } from '../react';
export class ReactPanelHeaderPart {
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
    focus() {
        //noop
    }
    init(parameters) {
        this.part = new ReactPart(this.element, this.reactPortalStore, this.component, {
            params: parameters.params,
            api: parameters.api,
            containerApi: parameters.containerApi,
            tabLocation: parameters.tabLocation,
        });
    }
    update(event) {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update({ params: event.params });
    }
    layout(_width, _height) {
        // noop - retrieval from api
    }
    dispose() {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    }
}
