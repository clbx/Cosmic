"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactPanelContentPart = void 0;
var react_1 = require("../react");
var dockview_core_1 = require("dockview-core");
var ReactPanelContentPart = /** @class */ (function () {
    function ReactPanelContentPart(id, component, reactPortalStore) {
        this.id = id;
        this.component = component;
        this.reactPortalStore = reactPortalStore;
        this._onDidFocus = new dockview_core_1.DockviewEmitter();
        this.onDidFocus = this._onDidFocus.event;
        this._onDidBlur = new dockview_core_1.DockviewEmitter();
        this.onDidBlur = this._onDidBlur.event;
        this._element = document.createElement('div');
        this._element.className = 'dv-react-part';
        this._element.style.height = '100%';
        this._element.style.width = '100%';
    }
    Object.defineProperty(ReactPanelContentPart.prototype, "element", {
        get: function () {
            return this._element;
        },
        enumerable: false,
        configurable: true
    });
    ReactPanelContentPart.prototype.focus = function () {
        // TODO
    };
    ReactPanelContentPart.prototype.init = function (parameters) {
        this.part = new react_1.ReactPart(this.element, this.reactPortalStore, this.component, {
            params: parameters.params,
            api: parameters.api,
            containerApi: parameters.containerApi,
        });
    };
    ReactPanelContentPart.prototype.update = function (event) {
        var _a;
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.update({ params: event.params });
    };
    ReactPanelContentPart.prototype.layout = function (_width, _height) {
        // noop
    };
    ReactPanelContentPart.prototype.dispose = function () {
        var _a;
        this._onDidFocus.dispose();
        this._onDidBlur.dispose();
        (_a = this.part) === null || _a === void 0 ? void 0 : _a.dispose();
    };
    return ReactPanelContentPart;
}());
exports.ReactPanelContentPart = ReactPanelContentPart;
