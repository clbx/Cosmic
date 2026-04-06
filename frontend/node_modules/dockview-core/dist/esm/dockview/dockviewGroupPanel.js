import { DockviewGroupPanelModel, } from './dockviewGroupPanelModel';
import { GridviewPanel } from '../gridview/gridviewPanel';
import { DockviewGroupPanelApiImpl, } from '../api/dockviewGroupPanelApi';
// GridConstraintChangeEvent2 is not exported, so we'll type it manually
const MINIMUM_DOCKVIEW_GROUP_PANEL_WIDTH = 100;
const MINIMUM_DOCKVIEW_GROUP_PANEL_HEIGHT = 100;
export class DockviewGroupPanel extends GridviewPanel {
    get minimumWidth() {
        var _a;
        // Check for explicitly set group constraint first
        if (typeof this._explicitConstraints.minimumWidth === 'number') {
            return this._explicitConstraints.minimumWidth;
        }
        const activePanelMinimumWidth = (_a = this.activePanel) === null || _a === void 0 ? void 0 : _a.minimumWidth;
        if (typeof activePanelMinimumWidth === 'number') {
            return activePanelMinimumWidth;
        }
        return super.__minimumWidth();
    }
    get minimumHeight() {
        var _a;
        // Check for explicitly set group constraint first
        if (typeof this._explicitConstraints.minimumHeight === 'number') {
            return this._explicitConstraints.minimumHeight;
        }
        const activePanelMinimumHeight = (_a = this.activePanel) === null || _a === void 0 ? void 0 : _a.minimumHeight;
        if (typeof activePanelMinimumHeight === 'number') {
            return activePanelMinimumHeight;
        }
        return super.__minimumHeight();
    }
    get maximumWidth() {
        var _a;
        // Check for explicitly set group constraint first
        if (typeof this._explicitConstraints.maximumWidth === 'number') {
            return this._explicitConstraints.maximumWidth;
        }
        const activePanelMaximumWidth = (_a = this.activePanel) === null || _a === void 0 ? void 0 : _a.maximumWidth;
        if (typeof activePanelMaximumWidth === 'number') {
            return activePanelMaximumWidth;
        }
        return super.__maximumWidth();
    }
    get maximumHeight() {
        var _a;
        // Check for explicitly set group constraint first
        if (typeof this._explicitConstraints.maximumHeight === 'number') {
            return this._explicitConstraints.maximumHeight;
        }
        const activePanelMaximumHeight = (_a = this.activePanel) === null || _a === void 0 ? void 0 : _a.maximumHeight;
        if (typeof activePanelMaximumHeight === 'number') {
            return activePanelMaximumHeight;
        }
        return super.__maximumHeight();
    }
    get panels() {
        return this._model.panels;
    }
    get activePanel() {
        return this._model.activePanel;
    }
    get size() {
        return this._model.size;
    }
    get model() {
        return this._model;
    }
    get locked() {
        return this._model.locked;
    }
    set locked(value) {
        this._model.locked = value;
    }
    get header() {
        return this._model.header;
    }
    constructor(accessor, id, options) {
        var _a, _b, _c, _d, _e, _f;
        super(id, 'groupview_default', {
            minimumHeight: (_b = (_a = options.constraints) === null || _a === void 0 ? void 0 : _a.minimumHeight) !== null && _b !== void 0 ? _b : MINIMUM_DOCKVIEW_GROUP_PANEL_HEIGHT,
            minimumWidth: (_d = (_c = options.constraints) === null || _c === void 0 ? void 0 : _c.minimumWidth) !== null && _d !== void 0 ? _d : MINIMUM_DOCKVIEW_GROUP_PANEL_WIDTH,
            maximumHeight: (_e = options.constraints) === null || _e === void 0 ? void 0 : _e.maximumHeight,
            maximumWidth: (_f = options.constraints) === null || _f === void 0 ? void 0 : _f.maximumWidth,
        }, new DockviewGroupPanelApiImpl(id, accessor));
        // Track explicitly set constraints to override panel constraints
        this._explicitConstraints = {};
        this.api.initialize(this); // cannot use 'this' after after 'super' call
        this._model = new DockviewGroupPanelModel(this.element, accessor, id, options, this);
        this.addDisposables(this.model.onDidActivePanelChange((event) => {
            this.api._onDidActivePanelChange.fire(event);
        }), this.api.onDidConstraintsChangeInternal((event) => {
            // Track explicitly set constraints to override panel constraints
            // Extract numeric values from functions or values
            if (event.minimumWidth !== undefined) {
                this._explicitConstraints.minimumWidth = typeof event.minimumWidth === 'function'
                    ? event.minimumWidth()
                    : event.minimumWidth;
            }
            if (event.minimumHeight !== undefined) {
                this._explicitConstraints.minimumHeight = typeof event.minimumHeight === 'function'
                    ? event.minimumHeight()
                    : event.minimumHeight;
            }
            if (event.maximumWidth !== undefined) {
                this._explicitConstraints.maximumWidth = typeof event.maximumWidth === 'function'
                    ? event.maximumWidth()
                    : event.maximumWidth;
            }
            if (event.maximumHeight !== undefined) {
                this._explicitConstraints.maximumHeight = typeof event.maximumHeight === 'function'
                    ? event.maximumHeight()
                    : event.maximumHeight;
            }
        }));
    }
    focus() {
        if (!this.api.isActive) {
            this.api.setActive();
        }
        super.focus();
    }
    initialize() {
        this._model.initialize();
    }
    setActive(isActive) {
        super.setActive(isActive);
        this.model.setActive(isActive);
    }
    layout(width, height) {
        super.layout(width, height);
        this.model.layout(width, height);
    }
    getComponent() {
        return this._model;
    }
    toJSON() {
        return this.model.toJSON();
    }
}
