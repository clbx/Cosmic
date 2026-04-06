"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactHeaderActionsRendererPart = void 0;
var react_1 = require("../react");
var dockview_core_1 = require("dockview-core");
var ReactHeaderActionsRendererPart = /** @class */ (function () {
    function ReactHeaderActionsRendererPart(component, reactPortalStore, _group) {
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._group = _group;
        this.mutableDisposable = new dockview_core_1.DockviewMutableDisposable();
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    Object.defineProperty(ReactHeaderActionsRendererPart.prototype, "element", {
        get: function () {
            return this._element;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(ReactHeaderActionsRendererPart.prototype, "part", {
        get: function () {
            return this._part;
        },
        enumerable: false,
        configurable: true
    });
    ReactHeaderActionsRendererPart.prototype.init = function (parameters) {
        var _this = this;
        this.mutableDisposable.value = new dockview_core_1.DockviewCompositeDisposable(this._group.model.onDidAddPanel(function () {
            _this.updatePanels();
        }), this._group.model.onDidRemovePanel(function () {
            _this.updatePanels();
        }), this._group.model.onDidActivePanelChange(function () {
            _this.updateActivePanel();
        }), parameters.api.onDidActiveChange(function () {
            _this.updateGroupActive();
        }));
        this._part = new react_1.ReactPart(this.element, this.reactPortalStore, this.component, {
            api: parameters.api,
            containerApi: parameters.containerApi,
            panels: this._group.model.panels,
            activePanel: this._group.model.activePanel,
            isGroupActive: this._group.api.isActive,
            group: this._group,
        });
    };
    ReactHeaderActionsRendererPart.prototype.dispose = function () {
        var _a;
        this.mutableDisposable.dispose();
        (_a = this._part) === null || _a === void 0 ? void 0 : _a.dispose();
    };
    ReactHeaderActionsRendererPart.prototype.update = function (event) {
        var _a;
        (_a = this._part) === null || _a === void 0 ? void 0 : _a.update(event.params);
    };
    ReactHeaderActionsRendererPart.prototype.updatePanels = function () {
        this.update({ params: { panels: this._group.model.panels } });
    };
    ReactHeaderActionsRendererPart.prototype.updateActivePanel = function () {
        this.update({
            params: {
                activePanel: this._group.model.activePanel,
            },
        });
    };
    ReactHeaderActionsRendererPart.prototype.updateGroupActive = function () {
        this.update({
            params: {
                isGroupActive: this._group.api.isActive,
            },
        });
    };
    return ReactHeaderActionsRendererPart;
}());
exports.ReactHeaderActionsRendererPart = ReactHeaderActionsRendererPart;
