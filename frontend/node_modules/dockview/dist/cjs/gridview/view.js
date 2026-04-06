"use strict";
var __extends = (this && this.__extends) || (function () {
    var extendStatics = function (d, b) {
        extendStatics = Object.setPrototypeOf ||
            ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
            function (d, b) { for (var p in b) if (Object.prototype.hasOwnProperty.call(b, p)) d[p] = b[p]; };
        return extendStatics(d, b);
    };
    return function (d, b) {
        if (typeof b !== "function" && b !== null)
            throw new TypeError("Class extends value " + String(b) + " is not a constructor or null");
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
Object.defineProperty(exports, "__esModule", { value: true });
exports.ReactGridPanelView = void 0;
var dockview_core_1 = require("dockview-core");
var react_1 = require("../react");
var ReactGridPanelView = /** @class */ (function (_super) {
    __extends(ReactGridPanelView, _super);
    function ReactGridPanelView(id, component, reactComponent, reactPortalStore) {
        var _this = _super.call(this, id, component) || this;
        _this.reactComponent = reactComponent;
        _this.reactPortalStore = reactPortalStore;
        return _this;
    }
    ReactGridPanelView.prototype.getComponent = function () {
        var _a, _b;
        return new react_1.ReactPart(this.element, this.reactPortalStore, this.reactComponent, {
            params: (_b = (_a = this._params) === null || _a === void 0 ? void 0 : _a.params) !== null && _b !== void 0 ? _b : {},
            api: this.api,
            // TODO: fix casting hack
            containerApi: new dockview_core_1.GridviewApi(this._params
                .accessor),
        });
    };
    return ReactGridPanelView;
}(dockview_core_1.GridviewPanel));
exports.ReactGridPanelView = ReactGridPanelView;
