import { ReactPart } from '../react';
import { DockviewCompositeDisposable, DockviewMutableDisposable, } from 'dockview-core';
export class ReactHeaderActionsRendererPart {
    get element() {
        return this._element;
    }
    get part() {
        return this._part;
    }
    constructor(component, reactPortalStore, _group) {
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._group = _group;
        this.mutableDisposable = new DockviewMutableDisposable();
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    init(parameters) {
        this.mutableDisposable.value = new DockviewCompositeDisposable(this._group.model.onDidAddPanel(() => {
            this.updatePanels();
        }), this._group.model.onDidRemovePanel(() => {
            this.updatePanels();
        }), this._group.model.onDidActivePanelChange(() => {
            this.updateActivePanel();
        }), parameters.api.onDidActiveChange(() => {
            this.updateGroupActive();
        }));
        this._part = new ReactPart(this.element, this.reactPortalStore, this.component, {
            api: parameters.api,
            containerApi: parameters.containerApi,
            panels: this._group.model.panels,
            activePanel: this._group.model.activePanel,
            isGroupActive: this._group.api.isActive,
            group: this._group,
        });
    }
    dispose() {
        var _a;
        this.mutableDisposable.dispose();
        (_a = this._part) === null || _a === void 0 ? void 0 : _a.dispose();
    }
    update(event) {
        var _a;
        (_a = this._part) === null || _a === void 0 ? void 0 : _a.update(event.params);
    }
    updatePanels() {
        this.update({ params: { panels: this._group.model.panels } });
    }
    updateActivePanel() {
        this.update({
            params: {
                activePanel: this._group.model.activePanel,
            },
        });
    }
    updateGroupActive() {
        this.update({
            params: {
                isGroupActive: this._group.api.isActive,
            },
        });
    }
}
